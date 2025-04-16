/**
 * File:   modbus_client.c
 * Author: AWTK Develop Team
 * Brief:  modbus client
 *
 * Copyright (c) 2023 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-09-30 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/url.h"
#include "tkc/time_now.h"
//#include "streams/inet/iostream_tcp.h"
#include "streams/serial/iostream_serial.h"
#include "streams/stream_factory.h"

#include "modbus_client.h"

#define MODBUS_CLIENT_DEFAULT_RETRY_TIMES 3

static ret_t modbus_client_deinit(modbus_client_t* client);

static uint32_t modbus_client_calculate_frame_gap_time(tk_iostream_t* io) {
  float_t frame_bits = 1;
  int32_t baudrate = tk_object_get_prop_uint32(TK_OBJECT(io), TK_IOSTREAM_SERIAL_PROP_BAUDRATE, 115200);
  uint8_t bytesize = tk_object_get_prop_uint32(TK_OBJECT(io), TK_IOSTREAM_SERIAL_PROP_BYTESIZE, (uint8_t)eightbits); 
  stopbits_t stopbits = (stopbits_t)tk_object_get_prop_uint32(TK_OBJECT(io), TK_IOSTREAM_SERIAL_PROP_STOPBITS, (uint8_t)stopbits_one); 
  frame_bits += bytesize;
  switch (stopbits) {
  case stopbits_one:
    frame_bits += 1.0f;
    break;
  case stopbits_two:
    frame_bits += 2.0f;
    break;
  case stopbits_one_point_five:
    frame_bits += 1.5f;
    break;
  default:
    frame_bits += 2.0f;
    break;
  }
  return ceil(3.5f * frame_bits * 1000 * 1000 / baudrate);
}

static ret_t modbus_client_init_with_io(modbus_client_t* client, tk_iostream_t* io, modbus_proto_t proto, uint32_t retry_times) {
  return_value_if_fail(client!= NULL && io != NULL, RET_BAD_PARAMS);
  tk_client_init(&(client->client), io, NULL);
  modbus_common_init(&client->common, io, proto, &(client->client.wb));
  modbus_client_set_retry_times(client, retry_times);
  client->is_connected = TRUE;
  return RET_OK;
}

modbus_client_t* modbus_client_create_with_io(tk_iostream_t* io, modbus_proto_t proto) {
  modbus_client_t* client = NULL;
  return_value_if_fail(io != NULL, NULL);

  client = TKMEM_ZALLOC(modbus_client_t);
  goto_error_if_fail(client != NULL);

  goto_error_if_fail(modbus_client_init_with_io(client, io, proto, MODBUS_CLIENT_DEFAULT_RETRY_TIMES) == RET_OK);

  return client;
error:
  TK_OBJECT_UNREF(io);

  return NULL;
}

static tk_iostream_t* modbus_client_create_iostream(const char* url) {
  tk_iostream_t* io = NULL;
  return_value_if_fail(url != NULL, NULL);
  if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ||
      tk_str_start_with(url, STR_SCHEMA_RTU_OVER_UDP)) {
    io = tk_stream_factory_create_iostream(url + 4 /*rtu+*/);
  } else {
    io = tk_stream_factory_create_iostream(url);
  }
  return io;
}

static ret_t modbus_client_init(modbus_client_t* client, const char* url, tk_iostream_t* io, uint8_t slave, uint32_t retry_times) {
  ret_t ret = RET_OK;
  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    ret = modbus_client_init_with_io(client, io, MODBUS_PROTO_TCP, retry_times);
    return_value_if_fail(ret == RET_OK, ret);
    modbus_client_set_slave(client, slave); 
  } else if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP)) {
    ret = modbus_client_init_with_io(client, io, MODBUS_PROTO_RTU, retry_times);
    return_value_if_fail(ret == RET_OK, ret);
  } else if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_UDP)) {
    ret = modbus_client_init_with_io(client, io, MODBUS_PROTO_RTU, retry_times);
    return_value_if_fail(ret == RET_OK, ret);
  } else if (tk_str_start_with(url, STR_SCHEMA_SERIAL)) {
    ret = modbus_client_init_with_io(client, io, MODBUS_PROTO_RTU, retry_times);
    return_value_if_fail(ret == RET_OK, ret);
    modbus_client_set_frame_gap_time(client, modbus_client_calculate_frame_gap_time(io));
  } else {
    log_debug("not support:%s\n", url);
  }
  return ret;
}

modbus_client_t* modbus_client_create(const char* url) {
  tk_iostream_t* io = NULL;
  modbus_client_t* client = NULL;
  return_value_if_fail(url != NULL, NULL);

  client = TKMEM_ZALLOC(modbus_client_t);
  return_value_if_fail(client != NULL, NULL);
  client->url = tk_strdup(url);
  goto_error_if_fail(client->url != NULL);

  io = modbus_client_create_iostream(client->url);
  goto_error_if_fail(io != NULL);

  goto_error_if_fail(modbus_client_init(client, client->url, io, 255, MODBUS_CLIENT_DEFAULT_RETRY_TIMES) == RET_OK);
  return client;
error :
  modbus_client_destroy(client);
  return NULL;
}

ret_t modbus_client_set_response_timeout(modbus_client_t* client, uint32_t response_timeout) {
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(client != NULL && common != NULL, RET_BAD_PARAMS);

  client->response_timeout = response_timeout;
  common->write_timeout = response_timeout;

  return RET_OK;
}

ret_t modbus_client_set_frame_gap_time(modbus_client_t* client, uint32_t frame_gap_time) {
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  client->frame_gap_time = tk_max(frame_gap_time, client->frame_gap_time);

  return RET_OK;
}

ret_t modbus_client_set_retry_times(modbus_client_t* client, uint32_t retry_times) {
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  client->retry_times = retry_times;

  return RET_OK;
}

static ret_t modbus_client_wait_for_frame_gap_time(modbus_client_t* client, uint64_t start_time) {
  uint64_t diff = time_now_us() - start_time;
  if (diff < client->frame_gap_time) {
    sleep_us(client->frame_gap_time - diff);
  }
  return RET_OK;
}

static ret_t modbus_client_check_and_set_recv_timeout(modbus_client_t* client, uint64_t start_time) {
  uint64_t diff = time_now_ms() - start_time;
  modbus_common_t* common = MODBUS_COMMON(client);
  if (client->response_timeout != 0) {
    if (diff >= client->response_timeout) {
      return RET_TIMEOUT;
    }
    common->read_timeout = client->response_timeout - diff;
  }
  return RET_OK;
}

static void modbus_client_check_connect_status(modbus_client_t* client, ret_t ret) {
  if (ret == RET_IO && client->is_connected) {
    client->is_connected = FALSE;
    modbus_client_deinit(client);
  }
}

static ret_t modbus_client_check_and_auto_connect(modbus_client_t* client) {
  if (client->is_connected) {
    return RET_OK;
  } else {
    modbus_common_t* common = MODBUS_COMMON(client);
    tk_iostream_t* io = modbus_client_create_iostream(client->url);
    return_value_if_fail(io != NULL, RET_IO);
    return modbus_client_init(client, client->url, io, common->slave, client->retry_times);
  }
}

static ret_t modbus_client_read_bits_ex(modbus_client_t* client, uint16_t func_code, uint16_t addr,
                                        uint16_t count, uint8_t* buff) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  t = time_now_ms();
  ret = modbus_common_send_read_bits_req(common, func_code, addr, count);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }
  t = time_now_us();
  ret = modbus_common_recv_read_bits_resp(common, func_code, buff, &count);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret;
}

static ret_t modbus_client_flush_read_buffer(modbus_client_t* client) {
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  log_debug("%s flush read buffer\n", __FUNCTION__);
  return modbus_common_flush_read_buffer(common);
}

#define MODBUS_NEED_RETRY(ret) ((ret) == RET_CRC)

ret_t modbus_client_read_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                              uint8_t* buff) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && buff != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_read_bits_ex(client, MODBUS_FC_READ_COILS, addr, count, buff);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_read_input_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                                    uint8_t* buff) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && buff != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_read_bits_ex(client, MODBUS_FC_READ_DISCRETE_INPUTS, addr, count, buff);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

static ret_t modbus_client_read_registers_ex(modbus_client_t* client, uint16_t func_code,
                                             uint16_t addr, uint16_t count, uint16_t* buff) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  t = time_now_ms();
  ret = modbus_common_send_read_registers_req(common, func_code, addr, count);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }
  t = time_now_us();
  ret = modbus_common_recv_read_registers_resp(common, func_code, buff, &count);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret;
}

ret_t modbus_client_read_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                   uint16_t* buff) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && buff != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_read_registers_ex(client, MODBUS_FC_READ_HOLDING_REGISTERS, addr, count,
                                          buff);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_read_input_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                         uint16_t* buff) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && buff != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret =
        modbus_client_read_registers_ex(client, MODBUS_FC_READ_INPUT_REGISTERS, addr, count, buff);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

static ret_t modbus_client_write_bit_impl(modbus_client_t* client, uint16_t addr, uint8_t value) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  t = time_now_ms();
  ret = modbus_common_send_write_bit_req(common, addr, value);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }
  t = time_now_us();
  ret = modbus_common_recv_write_bit_resp(common);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret;
}

static ret_t modbus_client_write_register_impl(modbus_client_t* client, uint16_t addr,
                                               uint16_t value) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  t = time_now_ms();
  ret = modbus_common_send_write_register_req(common, addr, value);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }

  t = time_now_us();
  ret = modbus_common_recv_write_register_resp(common);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret;
}

static ret_t modbus_client_write_bits_impl(modbus_client_t* client, uint16_t addr, uint16_t count,
                                           const uint8_t* buff) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  t = time_now_ms();
  ret = modbus_common_send_write_bits_req(common, addr, count, buff);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }

  t = time_now_us();
  ret = modbus_common_recv_write_bits_resp(common);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret;
}

static ret_t modbus_client_write_registers_impl(modbus_client_t* client, uint16_t addr,
                                                uint16_t count, const uint16_t* buff) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  t = time_now_ms();
  ret = modbus_common_send_write_registers_req(common, addr, count, buff);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }

  t = time_now_us();
  ret = modbus_common_recv_write_registers_resp(common);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret;
}

static ret_t modbus_client_write_and_read_registers_impl(modbus_client_t* client,
                                                         uint16_t write_addr, uint16_t write_nb, const uint16_t *src,
                                                         uint16_t read_addr, uint16_t read_nb, uint16_t *dest) {
  uint64_t t = 0;
  ret_t ret = RET_OK;
  uint16_t dest_count = read_nb;
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL && src != NULL && dest != NULL, RET_BAD_PARAMS);
  memset(dest, 0x0, read_nb);
  t = time_now_ms();
  ret = modbus_common_send_write_and_read_registers_req(common, write_addr, write_nb, src, read_addr, read_nb);
  return_value_if_fail(ret == RET_OK, ret);

  if (modbus_client_check_and_set_recv_timeout(client, t) != RET_OK) {
    return RET_TIMEOUT;
  }

  t = time_now_us();
  ret = modbus_common_recv_read_registers_resp(common, MODBUS_FC_WRITE_AND_READ_REGISTERS, dest, &dest_count);
  modbus_client_wait_for_frame_gap_time(client, t);
  return ret == RET_OK && read_nb == dest_count ? RET_OK : RET_FAIL;
}

ret_t modbus_client_write_bit(modbus_client_t* client, uint16_t addr, uint8_t value) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_write_bit_impl(client, addr, value);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_write_register(modbus_client_t* client, uint16_t addr, uint16_t value) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_write_register_impl(client, addr, value);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_write_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                               const uint8_t* buff) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && buff != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_write_bits_impl(client, addr, count, buff);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_write_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                    const uint16_t* buff) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && buff != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_write_registers_impl(client, addr, count, buff);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_write_and_read_registers(modbus_client_t* client, 
                                             uint16_t write_addr, uint16_t write_nb, const uint16_t *src,
                                             uint16_t read_addr, uint16_t read_nb, uint16_t *dest) {
  uint32_t i = 0;
  ret_t ret = RET_OK;
  return_value_if_fail(client != NULL && src != NULL && dest != NULL, RET_BAD_PARAMS);
  ret = modbus_client_check_and_auto_connect(client);
  if (ret != RET_OK) {
    return ret;
  }

  for (i = 0; i < client->retry_times; i++) {
    ret = modbus_client_write_and_read_registers_impl(client, write_addr, write_nb, src, read_addr, read_nb, dest);
    if (!MODBUS_NEED_RETRY(ret)) {
      modbus_client_check_connect_status(client, ret);
      return ret;
    }

    log_debug("%s retry:%d\n", __FUNCTION__, i + 1);
  }

  modbus_client_check_connect_status(client, ret);
  // clear the old resp data when comm error, ensure the next req and resp tid sync
  modbus_client_flush_read_buffer(client);
  return ret;
}

ret_t modbus_client_set_slave(modbus_client_t* client, uint8_t slave) {
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  common->slave = slave;

  return RET_OK;
}

ret_t modbus_client_set_auto_reconnect(modbus_client_t* client, bool_t auto_reconnect) {
  return_value_if_fail(client!= NULL, RET_BAD_PARAMS);
  client->auto_reconnect = auto_reconnect;
  return RET_OK;
}

static ret_t modbus_client_deinit(modbus_client_t* client) {
  modbus_common_t* common = MODBUS_COMMON(client);
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  tk_client_deinit(&(client->client));
  modbus_common_deinit(common);
  return RET_OK;
}

ret_t modbus_client_destroy(modbus_client_t* client) {
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  modbus_client_deinit(client);
  if (client->url != NULL) {
    TKMEM_FREE(client->url);
    client->url = NULL;
  }
  TKMEM_FREE(client);

  return RET_OK;
}


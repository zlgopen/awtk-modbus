/**
 * File:   modbus_client.c
 * Author: AWTK Develop Team
 * Brief:  modbus client
 *
 * Copyright (c) 2023 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "streams/inet/iostream_tcp.h"
#include "streams/serial/iostream_serial.h"
#include "streams/stream_factory.h"

#include "modbus_client.h"

modbus_client_t* modbus_client_create_with_io(tk_iostream_t* io, modbus_proto_t proto) {
  modbus_client_t* client = NULL;
  return_value_if_fail(io != NULL, NULL);

  client = TKMEM_ZALLOC(modbus_client_t);
  goto_error_if_fail(client != NULL);

  modbus_common_init(&client->common, io, proto);

  return client;
error:
  TK_OBJECT_UNREF(io);

  return NULL;
}

modbus_client_t* modbus_client_create(const char* url) {
  tk_iostream_t* io = NULL;
  modbus_client_t* client = NULL;
  return_value_if_fail(url != NULL, NULL);

  io = tk_stream_factory_create_iostream(url);
  return_value_if_fail(io != NULL, NULL);

  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  } else if (tk_str_start_with(url, STR_SCHEMA_SERIAL)) {
    client = modbus_client_create_with_io(io, MODBUS_PROTO_RTU);
  } else {
    log_debug("not support:%s\n", url);
  }

  return client;
}

static ret_t modbus_client_read_bits_ex(modbus_client_t* client, uint16_t func_code, uint16_t addr,
                                        uint16_t count, uint8_t* buff) {
  ret_t ret = RET_OK;
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  ret = modbus_common_send_read_bits_req(common, func_code, addr, count);
  return_value_if_fail(ret == RET_OK, ret);

  return modbus_common_recv_read_bits_resp(common, func_code, buff, &count);
}

ret_t modbus_client_read_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                              uint8_t* buff) {
  return modbus_client_read_bits_ex(client, MODBUS_FC_READ_COILS, addr, count, buff);
}

ret_t modbus_client_read_input_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                                    uint8_t* buff) {
  return modbus_client_read_bits_ex(client, MODBUS_FC_READ_DISCRETE_INPUTS, addr, count, buff);
}

static ret_t modbus_client_read_registers_ex(modbus_client_t* client, uint16_t func_code,
                                             uint16_t addr, uint16_t count, uint16_t* buff) {
  ret_t ret = RET_OK;
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  ret = modbus_common_send_read_registers_req(common, func_code, addr, count);
  return_value_if_fail(ret == RET_OK, ret);

  return modbus_common_recv_read_registers_resp(common, func_code, buff, &count);
}

ret_t modbus_client_read_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                   uint16_t* buff) {
  return modbus_client_read_registers_ex(client, MODBUS_FC_READ_HOLDING_REGISTERS, addr, count,
                                         buff);
}

ret_t modbus_client_read_input_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                         uint16_t* buff) {
  return modbus_client_read_registers_ex(client, MODBUS_FC_READ_INPUT_REGISTERS, addr, count, buff);
}

ret_t modbus_client_write_bit(modbus_client_t* client, uint16_t addr, uint8_t value) {
  ret_t ret = RET_OK;
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  ret = modbus_common_send_write_bit_req(common, addr, value);
  return_value_if_fail(ret == RET_OK, ret);

  return modbus_common_recv_write_bit_resp(common);
}

ret_t modbus_client_write_register(modbus_client_t* client, uint16_t addr, uint16_t value) {
  ret_t ret = RET_OK;
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  ret = modbus_common_send_write_register_req(common, addr, value);
  return_value_if_fail(ret == RET_OK, ret);

  return modbus_common_recv_write_register_resp(common);
}

ret_t modbus_client_write_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                               const uint8_t* buff) {
  ret_t ret = RET_OK;
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  ret = modbus_common_send_write_bits_req(common, addr, count, buff);
  return_value_if_fail(ret == RET_OK, ret);

  return modbus_common_recv_write_bits_resp(common);
}

ret_t modbus_client_write_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                    const uint16_t* buff) {
  ret_t ret = RET_OK;
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL && buff != NULL, RET_BAD_PARAMS);

  ret = modbus_common_send_write_registers_req(common, addr, count, buff);
  return_value_if_fail(ret == RET_OK, ret);

  return modbus_common_recv_write_registers_resp(common);
}

ret_t modbus_client_set_slave(modbus_client_t* client, uint8_t slave) {
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  common->slave = slave;

  return RET_OK;
}

ret_t modbus_client_destroy(modbus_client_t* client) {
  modbus_common_t* common = (modbus_common_t*)client;
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  modbus_common_deinit(common);
  TKMEM_FREE(client);

  return RET_OK;
}

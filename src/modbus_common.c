/**
 * File:   modbus_common.c
 * Author: AWTK Develop Team
 * Brief:  modbus_common
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
 * 2023-09-29 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/crc.h"
#include "tkc/time_now.h"
#include "modbus_common.h"
#define MODBUS_MAX_PAYLOAD 1024

static ret_t modbus_common_pack_uint16(modbus_common_t* common, uint16_t value) {
  return_value_if_fail(common != NULL && common->wbuffer != NULL, RET_BAD_PARAMS);
  value = TK_HTONS(value);
  return_value_if_fail(wbuffer_write_uint16(common->wbuffer, value) == RET_OK, RET_FAIL);

  return RET_OK;
}

static ret_t modbus_common_pack_uint8(modbus_common_t* common, uint8_t value) {
  return_value_if_fail(common != NULL && common->wbuffer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wbuffer_write_uint8(common->wbuffer, value) == RET_OK, RET_FAIL);

  return RET_OK;
}

static ret_t modbus_common_write_crc16(modbus_common_t* common) {
  uint16_t crc = 0;
  return_value_if_fail(common != NULL && common->wbuffer != NULL, RET_BAD_PARAMS);
  crc = tk_crc16_modbus(common->wbuffer->data, common->wbuffer->cursor);

  return wbuffer_write_uint16(common->wbuffer, crc);
}

static ret_t modbus_common_pack_header(modbus_common_t* common, uint8_t function_code,
                                       uint16_t data_len) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(common != NULL && common->wbuffer != NULL, RET_BAD_PARAMS);

  wb = common->wbuffer;
  wbuffer_rewind(wb);

  if (common->proto == MODBUS_PROTO_TCP) {
    /*For synchronization between messages of server and common*/
    modbus_common_pack_uint16(common, common->transaction_id);
    /*0 for Modbus/TCP*/
    modbus_common_pack_uint16(common, 0);
    /*Number of remaining bytes in this frame*/
    modbus_common_pack_uint16(common, data_len + 1 /*function_code*/ + 1 /*unit_id*/);
    /*Server address (255 if not used)*/
    wbuffer_write_uint8(wb, 0xff);
  } else {
    wbuffer_write_uint8(wb, common->slave);
  }
  /*Function code*/
  wbuffer_write_uint8(wb, function_code);

  return RET_OK;
}

static ret_t modbus_common_pack_tail(modbus_common_t* common) {
  return_value_if_fail(common != NULL && common->wbuffer != NULL, RET_BAD_PARAMS);

  if (common->proto == MODBUS_PROTO_RTU) {
    modbus_common_write_crc16(common);
  }

  return RET_OK;
}

ret_t modbus_common_init(modbus_common_t* common, tk_iostream_t* io, modbus_proto_t proto,
                         wbuffer_t* wb) {
  return_value_if_fail(common != NULL && io != NULL && wb != NULL, RET_BAD_PARAMS);

  common->io = io;
  common->proto = proto;
  common->transaction_id = 1;
  common->read_timeout = MODBUS_READ_TIMEOUT;
  common->write_timeout = MODBUS_WRITE_TIMEOUT;
  common->wbuffer = wb;

  return RET_OK;
}

static int32_t modbus_common_read_len(modbus_common_t* common, uint8_t* buff, uint32_t len) {
  return tk_iostream_read_len(common->io, buff, len, common->read_timeout);
}

static ret_t modbus_common_send_wbuffer(modbus_common_t* common) {
  const uint8_t* buff = common->wbuffer->data;
  uint32_t len = common->wbuffer->cursor;
  int32_t ret = tk_iostream_write_len(common->io, buff, len, common->write_timeout);

  return ret == len ? RET_OK : RET_IO;
}

static uint32_t modbus_common_get_resp_playload_length(modbus_common_t* common, uint8_t func_code) {
  switch (func_code) {
    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER:
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
      return 4;
    }
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS: {
      return 1;
    }
    default: {
      assert(!"not supported function code\n");
      return 1;
    }
  }
}

static ret_t modbus_common_check_crc(modbus_common_t* common, const uint8_t* data, uint32_t size) {
  if (common->proto == MODBUS_PROTO_RTU) {
    uint16_t get_crc = 0;
    uint16_t calc_crc = tk_crc16_modbus(data, size);
    int32_t len = modbus_common_read_len(common, (uint8_t*)&get_crc, 2);

    return (len != 2 || get_crc != calc_crc) ? RET_CRC : RET_OK;
  }

  return RET_OK;
}

static ret_t modbus_common_read_rtu_header(modbus_common_t* common, modbus_rtu_header_t* header,
                                      uint8_t expected_func_code) {
  int32_t ret = 0;
  uint8_t slave = 0;
  uint8_t func_code = 0;
  int32_t cost_time = 0;
  uint64_t start_time = time_now_ms();
  return_value_if_fail(common != NULL && header != NULL, RET_BAD_PARAMS);

  do {
    do {
      ret = modbus_common_read_len(common, &slave, 1);
      return_value_if_fail(ret == 1, RET_IO);

      if (slave == common->slave) {
        break;
      } else {
        log_debug("skip invalid slave %d\n", slave);
      }
      cost_time = time_now_ms() - start_time;
      if (cost_time > common->read_timeout) {
        log_debug("read data timeout\n");
        return RET_FAIL;
      }
    } while (1);

    ret = modbus_common_read_len(common, &func_code, 1);
    return_value_if_fail(ret == 1, RET_IO);

    if (expected_func_code != 0) {
      if (func_code == expected_func_code || (func_code == (expected_func_code | 0x80))) {
        header->slave = slave;
        header->func_code = func_code;
        return RET_OK;
      } else {
        log_debug("skip invalid func_code %d\n", func_code);
      }
    } else {
      switch (func_code) {
        case MODBUS_FC_READ_COILS:
        case MODBUS_FC_READ_DISCRETE_INPUTS:
        case MODBUS_FC_READ_HOLDING_REGISTERS:
        case MODBUS_FC_READ_INPUT_REGISTERS:
        case MODBUS_FC_WRITE_SINGLE_COIL:
        case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER:
        case MODBUS_FC_WRITE_MULTIPLE_COILS:
        case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS:
        case MODBUS_FC_WRITE_AND_READ_REGISTERS: {
          header->slave = slave;
          header->func_code = func_code;
          return RET_OK;
        }
        default: {
          log_debug("skip invalid func_code %d\n", func_code);
          break;
        }
      }
    }
    cost_time = time_now_ms() - start_time;
    if (cost_time > common->read_timeout) {
      log_debug("read data timeout\n");
      return RET_FAIL;
    }
  } while (1);

  return RET_FAIL;
}

static ret_t modbus_common_recv_resp(modbus_common_t* common, uint8_t expected_func_code,
                                     modbus_resp_data_t* resp) {
  int32_t len = 0;
  uint8_t bytes = 0;
  uint8_t func_code = 0;
  wbuffer_t* wb = NULL;
  uint8_t* buff = NULL;
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);

  wb = common->wbuffer;
  wbuffer_rewind(wb);
  buff = wb->data;

  if (common->proto == MODBUS_PROTO_TCP) {
    uint16_t transaction_id = 0;
    modbus_tcp_header_t* header = (modbus_tcp_header_t*)buff;
    int32_t ret = modbus_common_read_len(common, buff, sizeof(*header));
    return_value_if_fail(ret == sizeof(*header), RET_IO);
    wbuffer_skip(wb, sizeof(*header));
    func_code = header->func_code;
    return_value_if_fail(header->unit_id == 0xff, RET_IO);
    return_value_if_fail(header->protocol_id == 0, RET_IO);

    transaction_id = TK_HTONS(header->transaction_id);
    return_value_if_fail(transaction_id == common->transaction_id, RET_IO);
  } else {
    modbus_rtu_header_t* header = (modbus_rtu_header_t*)buff;
    ret_t ret = modbus_common_read_rtu_header(common, header, expected_func_code);
    return_value_if_fail(ret == RET_OK, ret);

    wbuffer_skip(wb, sizeof(*header));
    func_code = header->func_code;
  }

  if (func_code == expected_func_code) {
    bytes = modbus_common_get_resp_playload_length(common, expected_func_code);
    return_value_if_fail((bytes + wb->cursor + 1) < MODBUS_MAX_PAYLOAD, RET_IO);

    wbuffer_extend_capacity(wb, bytes + wb->cursor + 1);
    buff = wb->data + wb->cursor;
    len = modbus_common_read_len(common, buff, bytes);
    return_value_if_fail(len == bytes, RET_IO);
    wbuffer_skip(wb, bytes);

    /*对于读请求，读取后面的数据*/
    if (bytes == 1) {
      bytes = *buff;
      buff = wb->data + wb->cursor;
      len = modbus_common_read_len(common, buff, bytes);
      return_value_if_fail(len == bytes, RET_IO);
      wbuffer_skip(wb, bytes);

      if (resp != NULL) {
        resp->func_code = func_code;
        resp->bytes = bytes;
        resp->data = buff;
      }
    }

    if (modbus_common_check_crc(common, wb->data, wb->cursor) != RET_OK) {
      return RET_CRC;
    }

    return RET_OK;
  } else if (func_code == (expected_func_code | 0x80)) {
    uint8_t exception_code = 0;
    len = modbus_common_read_len(common, &exception_code, 1);
    common->last_exception_code = (modbus_exeption_code_t)exception_code;
    log_debug("%d: %s\n", expected_func_code, modbus_common_get_last_exception_str(common));
    if (modbus_common_check_crc(common, wb->data, wb->cursor) != RET_OK) {
      return RET_CRC;
    }

    return RET_FAIL;
  } else {
    return RET_FAIL;
  }
}

ret_t modbus_common_send_read_req(modbus_common_t* common, uint16_t func_code, uint16_t addr,
                                  uint16_t count) {
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);

  common->transaction_id++;
  modbus_common_pack_header(common, func_code, 4);
  modbus_common_pack_uint16(common, addr);
  modbus_common_pack_uint16(common, count);
  modbus_common_pack_tail(common);

  return modbus_common_send_wbuffer(common);
}

static ret_t modbus_common_decode_bits(modbus_resp_data_t* resp, uint8_t* buffer, uint16_t n_bits) {
  uint16_t i = 0;
  uint16_t n = resp->bytes;
  uint8_t* p = resp->data;
  uint16_t n_bytes = modbus_bits_to_bytes(n_bits);

  return_value_if_fail(n == n_bytes, RET_BAD_PARAMS);

  /*每个位用一个字节表示*/
  for (i = 0; i < n_bits; i++) {
    bool_t v = FALSE;
    bits_stream_get(p, n, i, &v);
    buffer[i] = v;
  }

  return RET_OK;
}

static ret_t modbus_common_encode_bits(wbuffer_t* wb, const uint8_t* buffer, uint16_t n_bits) {
  uint16_t i = 0;
  uint16_t offset = wb->cursor;
  uint16_t n = n_bits / 8 + (n_bits % 8 ? 1 : 0);
  uint8_t* data = wb->data + offset + 1;

  wb->data[offset] = n;
  memset(data, 0, n);
  for (i = 0; i < n_bits; i++) {
    bool_t v = buffer[i];
    bits_stream_set(data, n, i, v);
  }
  wbuffer_skip(wb, n + 1);

  return RET_OK;
}

static ret_t modbus_common_encode_registers(wbuffer_t* wb, const uint16_t* buffer,
                                            uint16_t n_registers) {
  uint16_t i = 0;
  uint16_t offset = wb->cursor;
  uint16_t n = n_registers * 2;
  uint8_t* data = wb->data + offset + 1;

  wb->data[offset] = n;
  for (i = 0; i < n_registers; i++) {
    uint16_t v = buffer[i];
    data[i * 2] = v >> 8;
    data[i * 2 + 1] = v & 0xff;
  }
  wbuffer_skip(wb, n + 1);

  return RET_OK;
}

static ret_t modbus_common_decode_registers(modbus_resp_data_t* resp, uint16_t* buffer,
                                            uint16_t n_registers) {
  uint16_t i = 0;
  uint8_t n = resp->bytes;
  uint8_t* p = resp->data;
  return_value_if_fail(n_registers * 2 == n, RET_BAD_PARAMS);

  for (i = 0; i < n_registers; i++) {
    buffer[i] = p[0] << 8 | p[1];
    p += 2;
  }
  return RET_OK;
}

ret_t modbus_common_send_read_bits_req(modbus_common_t* common, uint16_t func_code, uint16_t addr,
                                       uint16_t count) {
  return modbus_common_send_read_req(common, func_code, addr, count);
}

ret_t modbus_common_recv_read_bits_resp(modbus_common_t* common, uint16_t func_code,
                                        uint8_t* buffer, uint16_t* count) {
  ret_t ret = RET_OK;
  uint16_t n_bits = 0;
  modbus_resp_data_t resp;
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buffer != NULL && count != NULL, RET_BAD_PARAMS);

  n_bits = *count;
  memset(&resp, 0x00, sizeof(resp));
  ret = modbus_common_recv_resp(common, func_code, &resp);
  if (ret != RET_OK) {
    return ret;
  }

  return modbus_common_decode_bits(&resp, buffer, n_bits);
}

ret_t modbus_common_send_read_registers_req(modbus_common_t* common, uint16_t func_code,
                                            uint16_t addr, uint16_t count) {
  return modbus_common_send_read_req(common, func_code, addr, count);
}

ret_t modbus_common_recv_read_registers_resp(modbus_common_t* common, uint16_t func_code,
                                             uint16_t* buffer, uint16_t* count) {
  ret_t ret = RET_OK;
  uint16_t n_registers = 0;
  modbus_resp_data_t resp;

  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buffer != NULL && count != NULL, RET_BAD_PARAMS);

  n_registers = *count;
  memset(&resp, 0x00, sizeof(resp));
  ret = modbus_common_recv_resp(common, func_code, &resp);
  if (ret != RET_OK) {
    return ret;
  }
  return modbus_common_decode_registers(&resp, buffer, n_registers);
}

static ret_t modbus_common_send_write_single_req(modbus_common_t* common, uint8_t func_code,
                                                 uint16_t addr, uint16_t value) {
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);

  common->transaction_id++;
  modbus_common_pack_header(common, func_code, 4);
  modbus_common_pack_uint16(common, addr);
  modbus_common_pack_uint16(common, value);
  modbus_common_pack_tail(common);

  return modbus_common_send_wbuffer(common);
}

ret_t modbus_common_send_write_bit_req(modbus_common_t* common, uint16_t addr, uint8_t value) {
  return modbus_common_send_write_single_req(common, MODBUS_FC_WRITE_SINGLE_COIL, addr,
                                             value ? 0xff00 : 0);
}

ret_t modbus_common_recv_write_bit_resp(modbus_common_t* common) {
  return modbus_common_recv_resp(common, MODBUS_FC_WRITE_SINGLE_COIL, NULL);
}

ret_t modbus_common_send_write_register_req(modbus_common_t* common, uint16_t addr,
                                            uint16_t value) {
  return modbus_common_send_write_single_req(common, MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER, addr,
                                             value);
}

ret_t modbus_common_recv_write_register_resp(modbus_common_t* common) {
  return modbus_common_recv_resp(common, MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER, NULL);
}

static ret_t modbus_common_send_write_multi_req(modbus_common_t* common, uint8_t func_code,
                                                uint16_t addr, uint16_t count,
                                                const uint8_t* buffer) {
  bool_t is_write_bits = func_code == MODBUS_FC_WRITE_MULTIPLE_COILS;
  uint32_t bytes = is_write_bits ? modbus_bits_to_bytes(count) : count * 2;
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);

  common->transaction_id++;
  modbus_common_pack_header(common, func_code, bytes + 5);
  modbus_common_pack_uint16(common, addr);
  modbus_common_pack_uint16(common, count);

  if (is_write_bits) {
    modbus_common_encode_bits(common->wbuffer, buffer, count);
  } else {
    modbus_common_encode_registers(common->wbuffer, (uint16_t*)buffer, count);
  }

  modbus_common_pack_tail(common);

  return modbus_common_send_wbuffer(common);
}

ret_t modbus_common_send_write_bits_req(modbus_common_t* common, uint16_t addr, uint16_t count,
                                        const uint8_t* buffer) {
  return modbus_common_send_write_multi_req(common, MODBUS_FC_WRITE_MULTIPLE_COILS, addr, count,
                                            buffer);
}

ret_t modbus_common_recv_write_bits_resp(modbus_common_t* common) {
  return modbus_common_recv_resp(common, MODBUS_FC_WRITE_MULTIPLE_COILS, NULL);
}

ret_t modbus_common_send_write_registers_req(modbus_common_t* common, uint16_t addr, uint16_t count,
                                             const uint16_t* buffer) {
  return modbus_common_send_write_multi_req(common, MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS,
                                            addr, count, (uint8_t*)buffer);
}

ret_t modbus_common_recv_write_registers_resp(modbus_common_t* common) {
  return modbus_common_recv_resp(common, MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS, NULL);
}

ret_t modbus_common_send_write_and_read_registers_req(modbus_common_t* common, uint16_t write_addr,
                                                      uint16_t write_nb, const uint16_t* src,
                                                      uint16_t read_addr, uint16_t read_nb) {
  common->transaction_id++;
  modbus_common_pack_header(common, MODBUS_FC_WRITE_AND_READ_REGISTERS, 8 + write_nb * 2 + 1);
  modbus_common_pack_uint16(common, read_addr);
  modbus_common_pack_uint16(common, read_nb);
  modbus_common_pack_uint16(common, write_addr);
  modbus_common_pack_uint16(common, write_nb);
  modbus_common_encode_registers(common->wbuffer, (uint16_t*)src, write_nb);

  modbus_common_pack_tail(common);
  return modbus_common_send_wbuffer(common);
}

ret_t modbus_common_deinit(modbus_common_t* common) {
  return_value_if_fail(common != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

const char* modbus_common_get_exception_meaning(modbus_exeption_code_t exception_code) {
  switch (exception_code) {
    case MODBUS_EXCEPTION_ILLEGAL_FUNCTION:
      return "非法功能";
    case MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS:
      return "非法数据地址";
    case MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE:
      return "非法数据值";
    case MODBUS_EXCEPTION_SERVER_DEVICE_FAILURE:
      return "从站设备故障";
    case MODBUS_EXCEPTION_ACKNOWLEDGE:
      return "确认";
    case MODBUS_EXCEPTION_SERVER_DEVICE_BUSY:
      return "从站设备忙";
    case MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE:
      return "否认";
    case MODBUS_EXCEPTION_MEMORY_PARITY_ERROR:
      return "内存奇偶校验错误";
    case MODBUS_EXCEPTION_GATEWAY_PATH_UNAVAILABLE:
      return "网关路径不可用";
    case MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND:
      return "网关目标设备无响应";
    default:
      return "未知异常";
  }
}

modbus_exeption_code_t modbus_common_get_last_exception_code(modbus_common_t* common) {
  return_value_if_fail(common != NULL, MODBUS_EXCEPTION_ILLEGAL_FUNCTION);

  return common->last_exception_code;
}

const char* modbus_common_get_last_exception_str(modbus_common_t* common) {
  return_value_if_fail(common != NULL, NULL);

  return modbus_common_get_exception_meaning(common->last_exception_code);
}

/*for server side*/
ret_t modbus_common_recv_req(modbus_common_t* common, modbus_req_data_t* req_data) {
  int32_t ret = 0;
  uint8_t bytes = 0;
  uint8_t func_code = 0;
  wbuffer_t* wb = NULL;
  uint8_t* buff = NULL;
  uint16_t addr = 0;
  uint16_t count = 0;
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(req_data != NULL, RET_BAD_PARAMS);

  wb = common->wbuffer;
  wbuffer_rewind(wb);
  buff = wb->data;

  if (common->proto == MODBUS_PROTO_TCP) {
    uint16_t transaction_id = 0;
    modbus_tcp_header_t* header = (modbus_tcp_header_t*)buff;
    ret = modbus_common_read_len(common, buff, sizeof(*header));
    if (ret == 0) {
      return RET_EOS;
    }
    return_value_if_fail(ret == sizeof(*header), RET_IO);
    wbuffer_skip(wb, sizeof(*header));
    func_code = header->func_code;
    return_value_if_fail(header->unit_id == 0xff, RET_FAIL);
    return_value_if_fail(header->protocol_id == 0, RET_FAIL);
    transaction_id = TK_HTONS(header->transaction_id);
    return_value_if_fail(transaction_id > common->transaction_id || transaction_id == 0, RET_FAIL);
    common->transaction_id = TK_HTONS(header->transaction_id);
    req_data->slave = header->unit_id;
  } else {
    modbus_rtu_header_t* header = (modbus_rtu_header_t*)buff;
    ret = modbus_common_read_rtu_header(common, header, 0);
    return_value_if_fail(ret == RET_OK, ret);
    
    wbuffer_skip(wb, sizeof(*header));
    func_code = header->func_code;
    req_data->slave = header->slave;
  }

  req_data->func_code = func_code;
  buff = wb->data + wb->cursor;

  switch (func_code) {
    case MODBUS_FC_WRITE_AND_READ_REGISTERS: {
      ret = modbus_common_read_len(common, buff, 9);
      return_value_if_fail(ret == 9, RET_IO);
      wbuffer_skip(wb, 4);
      addr = buff[0] << 8 | buff[1];
      count = buff[2] << 8 | buff[3];
      req_data->addr = addr;
      req_data->count = count;
      req_data->data = NULL;
      req_data->bytes = 0;
      addr = buff[4] << 8 | buff[5];
      count = buff[6] << 8 | buff[7];
      req_data->addr_ex = addr;
      req_data->count_ex = count;
      req_data->bytes_ex = buff[8];

      buff = wb->data + wb->cursor;
      req_data->data_ex = buff;

      ret = modbus_common_read_len(common, buff, req_data->bytes_ex);
      return_value_if_fail(ret == req_data->bytes_ex, RET_IO);
      wbuffer_skip(wb, req_data->bytes_ex);
      break;
    }
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS: {
      ret = modbus_common_read_len(common, buff, 4);
      return_value_if_fail(ret == 4, RET_IO);
      wbuffer_skip(wb, 4);
      addr = buff[0] << 8 | buff[1];
      count = buff[2] << 8 | buff[3];
      req_data->addr = addr;
      req_data->count = count;
      req_data->data = NULL;
      req_data->bytes = 0;
      break;
    }
    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER: {
      ret = modbus_common_read_len(common, buff, 4);
      return_value_if_fail(ret == 4, RET_IO);
      wbuffer_skip(wb, 4);
      addr = buff[0] << 8 | buff[1];
      req_data->addr = addr;
      req_data->count = 1;
      req_data->data = buff + 2;
      req_data->bytes = 2;
      break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
      ret = modbus_common_read_len(common, buff, 5);
      return_value_if_fail(ret == 5, RET_IO);
      wbuffer_skip(wb, 5);
      addr = buff[0] << 8 | buff[1];
      count = buff[2] << 8 | buff[3];
      bytes = buff[4];
      req_data->addr = addr;
      req_data->count = count;
      req_data->data = buff + 5;
      req_data->bytes = bytes;

      buff = wb->data + wb->cursor;
      ret = modbus_common_read_len(common, buff, bytes);
      return_value_if_fail(ret == bytes, RET_IO);
      wbuffer_skip(wb, bytes);
      break;
    }
    default: {
      return RET_NOT_IMPL;
    }
  }

  if (modbus_common_check_crc(common, wb->data, wb->cursor) != RET_OK) {
    return RET_CRC;
  }

  return RET_OK;
}

ret_t modbus_common_send_resp(modbus_common_t* common, modbus_resp_data_t* resp_data) {
  wbuffer_t* wb = NULL;
  uint8_t func_code = 0;
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(resp_data != NULL, RET_BAD_PARAMS);

  wb = common->wbuffer;
  func_code = resp_data->func_code;
  switch (func_code) {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS: {
      uint8_t bytes = resp_data->bytes;
      uint16_t data_len = 1 + bytes;

      modbus_common_pack_header(common, func_code, data_len);
      modbus_common_pack_uint8(common, bytes);
      wbuffer_write_binary(wb, resp_data->data, bytes);
      break;
    }
    case MODBUS_FC_WRITE_AND_READ_REGISTERS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS: {
      uint8_t bytes = resp_data->count * 2;
      uint16_t data_len = 1 + bytes;
      modbus_common_pack_header(common, func_code, data_len);
      modbus_common_pack_uint8(common, bytes);
      wbuffer_write_binary(wb, resp_data->data, bytes);
      break;
    }
    case MODBUS_FC_WRITE_SINGLE_COIL: {
      uint16_t data_len = 4;
      modbus_common_pack_header(common, func_code, data_len);
      modbus_common_pack_uint16(common, resp_data->addr);
      modbus_common_pack_uint16(common, resp_data->data[0] ? 0xFF00 : 0x0);
      break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
      uint16_t data_len = 4;
      modbus_common_pack_header(common, func_code, data_len);
      modbus_common_pack_uint16(common, resp_data->addr);
      modbus_common_pack_uint16(common, resp_data->count);
      break;
    }
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER: {
      uint16_t data_len = 4;
      modbus_common_pack_header(common, func_code, data_len);
      modbus_common_pack_uint16(common, resp_data->addr);
      modbus_common_pack_uint8(common, resp_data->data[0]);
      modbus_common_pack_uint8(common, resp_data->data[1]);
      break;
    }
    default:
      break;
  }

  modbus_common_pack_tail(common);

  return modbus_common_send_wbuffer(common);
}

ret_t modbus_common_send_exception_resp(modbus_common_t* common, uint8_t func_code,
                                        modbus_exeption_code_t code) {
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);
  modbus_common_pack_header(common, func_code | 0x80, 2);
  modbus_common_pack_uint8(common, code);
  modbus_common_pack_tail(common);

  log_debug("send exception resp: %d %s\n", (int)func_code,
            modbus_common_get_exception_meaning(code));

  return modbus_common_send_wbuffer(common);
}

ret_t modbus_common_flush_read_buffer(modbus_common_t* common) {
  return_value_if_fail(common != NULL && common->io != NULL, RET_BAD_PARAMS);

  uint8_t flush_buffer[260];
  while (tk_iostream_read_len(common->io, flush_buffer, sizeof(flush_buffer), 0) > 0) {
    ;
  }
  return RET_OK;
}

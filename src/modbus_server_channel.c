/**
 * File:   modbus_server_channel.c
 * Author: AWTK Develop Team
 * Brief:  modbus_server_channel
 *
 * Copyright (c) 2023 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-3-10 Li XianJing <lixianjing@zlg.cn> created.
 *
 */

#include "tkc/mem.h"
#include "modbus_server_channel.h"

static ret_t modbus_server_channel_init(modbus_server_channel_t* channel) {
  uint32_t bytes = 0;
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  if (strstr(channel->name, MODBUS_SERVER_CHANNEL_BITS) != NULL ||
      strstr(channel->name, MODBUS_SERVER_CHANNEL_INPUT_BITS) != NULL) {
    bytes = tk_bits_to_bytes(channel->length);
  } else {
    bytes = channel->length * sizeof(uint16_t);
  }
  return_value_if_fail(bytes > 0, RET_BAD_PARAMS);

  channel->bytes = bytes;
  channel->data = TKMEM_ALLOC(bytes);
  return_value_if_fail(channel->data != NULL, RET_OOM);

  memset(channel->data, 0, bytes);

  return RET_OK;
}

ret_t modbus_server_channel_destroy(modbus_server_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(channel->data);
  TKMEM_FREE(channel->name);
  TKMEM_FREE(channel);

  return RET_OK;
}

modbus_server_channel_t* modbus_server_channel_create(const char* name, uint32_t start,
                                                      uint32_t length, bool_t writable) {
  modbus_server_channel_t* channel = TKMEM_ZALLOC(modbus_server_channel_t);
  return_value_if_fail(channel != NULL, NULL);

  channel->name = tk_strdup(name);
  channel->start = start;
  channel->length = length;
  channel->writable = writable;

  modbus_server_channel_init(channel);

  return channel;
}

modbus_server_channel_t* modbus_server_channel_create_with_conf(conf_node_t* node) {
  modbus_server_channel_t* channel = TKMEM_ZALLOC(modbus_server_channel_t);
  return_value_if_fail(node != NULL, NULL);
  return_value_if_fail(channel != NULL, NULL);

  channel->name = tk_strdup(conf_node_get_child_value_str(node, "name", NULL));
  channel->start = conf_node_get_child_value_int32(node, "start", 0);
  channel->length = conf_node_get_child_value_int32(node, "length", 0);
  channel->writable = conf_node_get_child_value_bool(node, "writable", FALSE);

  modbus_server_channel_init(channel);

  return channel;
}

static ret_t modbus_server_channel_read_bit(modbus_server_channel_t* channel, uint16_t addr,
                                            uint8_t* buff, uint16_t offset) {
  bool_t value = FALSE;
  uint16_t dst_bytes = tk_bits_to_bytes(offset) + 1;
  uint16_t src_offset = addr - channel->start;

  bits_stream_get(channel->data, channel->bytes, src_offset, &value);

  return bits_stream_set(buff, dst_bytes, offset, value);
}

ret_t modbus_server_channel_read_bits(modbus_server_channel_t* channel, uint16_t addr,
                                      uint16_t count, uint8_t* buff) {
  uint16_t i = 0;
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_READ_BITS, RET_INVALID_ADDR);
  return_value_if_fail(channel != NULL && channel->data != NULL && buff != NULL, RET_BAD_PARAMS);

  /*检测地址和范围是否合法*/
  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("%s read_bits invalid addr: addr:%d, count:%d, start:%d, length:%d\n", channel->name,
              (int)addr, (int)count, (int)(channel->start), (int)(channel->length));
    return RET_INVALID_ADDR;
  }

  /*读取数据*/
  for (i = 0; i < count; i++) {
    modbus_server_channel_read_bit(channel, addr + i, buff, i);
  }

  return RET_OK;
}

static ret_t modbus_server_channel_read_register(modbus_server_channel_t* channel, uint16_t addr,
                                                 uint16_t* buff) {
  uint16_t offset = addr - channel->start;
  uint16_t* data = (uint16_t*)channel->data;

  ENSURE(offset < channel->length);
  *buff = int16_to_big_endian(data[offset]);

  return RET_OK;
}

ret_t modbus_server_channel_read_registers(modbus_server_channel_t* channel, uint16_t addr,
                                           uint16_t count, uint16_t* buff) {
  uint16_t i = 0;
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_READ_REGISTERS, RET_INVALID_ADDR);
  return_value_if_fail(channel != NULL && channel->data != NULL && buff != NULL, RET_BAD_PARAMS);

  /*检测地址和范围是否合法*/
  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("%s read_registers invalid addr: addr:%d, count:%d, start:%d, length:%d\n", channel->name,
              (int)addr, (int)count, (int)(channel->start), (int)(channel->length));
    return RET_INVALID_ADDR;
  }

  /*准备数据*/
  for (i = 0; i < count; i++) {
    modbus_server_channel_read_register(channel, addr + i, buff + i);
  }

  return RET_OK;
}

ret_t modbus_server_channel_write_bit(modbus_server_channel_t* channel, uint16_t addr,
                                      uint8_t value) {
  return_value_if_fail(channel != NULL && channel->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(channel->writable, RET_BAD_PARAMS);

  /*检测地址是否合法*/
  if (addr < channel->start || addr >= (channel->start + channel->length)) {
    log_debug("%s write bit invalid addr: addr:%d, count:%d, start:%d, length:%d\n", channel->name,
              (int)addr, (int)1, (int)(channel->start), (int)(channel->length));
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  return bits_stream_set(channel->data, channel->bytes, addr - channel->start, value);
}

ret_t modbus_server_channel_write_bits(modbus_server_channel_t* channel, uint16_t addr,
                                       uint16_t count, const uint8_t* buff) {
  uint16_t i = 0;
  bool_t value = FALSE;
  uint16_t bytes = tk_bits_to_bytes(count);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(channel->writable, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_WRITE_BITS, RET_INVALID_ADDR);
  return_value_if_fail(channel != NULL && channel->data != NULL && buff != NULL, RET_BAD_PARAMS);

  /*检测地址和范围是否合法*/
  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("%s write bits invalid addr: addr:%d, count:%d, start:%d, length:%d\n", channel->name,
              (int)addr, (int)count, (int)(channel->start), (int)(channel->length));
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  for (i = 0; i < count; i++) {
    ENSURE(bits_stream_get(buff, bytes, i, &value) == RET_OK);
    modbus_server_channel_write_bit(channel, addr + i, value);
  }

  return RET_OK;
}

ret_t modbus_server_channel_write_register(modbus_server_channel_t* channel, uint16_t addr,
                                           uint16_t value) {
  uint16_t* data = NULL;
  return_value_if_fail(channel != NULL && channel->data != NULL, RET_BAD_PARAMS);
  return_value_if_fail(channel->writable, RET_BAD_PARAMS);

  data = (uint16_t*)channel->data;
  /*检测地址是否合法*/
  if (addr < channel->start || addr >= (channel->start + channel->length)) {
    log_debug("%s write register invalid addr: addr:%d, count:%d, start:%d, length:%d\n", channel->name,
              (int)addr, (int)1, (int)(channel->start), (int)(channel->length));
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  data[addr - channel->start] = int16_from_big_endian(value);

  return RET_OK;
}

ret_t modbus_server_channel_write_registers(modbus_server_channel_t* channel, uint16_t addr,
                                            uint16_t count, const uint16_t* buff) {
  uint32_t i = 0;
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(channel->writable, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_WRITE_REGISTERS, RET_INVALID_ADDR);
  return_value_if_fail(channel != NULL && channel->data != NULL && buff != NULL, RET_BAD_PARAMS);

  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("%s write registers invalid addr: addr:%d, count:%d, start:%d, length:%d\n", channel->name,
              (int)addr, (int)count, (int)(channel->start), (int)(channel->length));
    return RET_INVALID_ADDR;
  }

  for (i = 0; i < count; i++) {
    modbus_server_channel_write_register(channel, addr + i, buff[i]);
  }

  return RET_OK;
}

/**
 * File:   modbus_memory_default.c
 * Author: AWTK Develop Team
 * Brief:  modbus memory default
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
 * 2024-03-10 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "modbus_memory_default.h"

static ret_t modbus_memory_default_read_bit_from_channel(modbus_server_channel_t* channel,
                                                         uint16_t addr, uint8_t* buff,
                                                         uint16_t offset) {
  bool_t value = FALSE;
  uint16_t bytes = offset / 8 + 1;
  uint8_t* data = channel->data;
  uint16_t src_offset = addr - channel->start;

  ENSURE(bits_stream_get(data, bytes, src_offset, &value) == RET_OK);

  return bits_stream_set(buff, bytes, offset, value);
}

static ret_t modbus_memory_default_read_bits_from_channel(modbus_server_channel_t* channel,
                                                          uint16_t addr, uint16_t count,
                                                          uint8_t* buff) {
  uint16_t i = 0;
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  /*检测地址和范围是否合法*/
  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("invalid addr: addr:%d, count:%d, start:%d, length:%d\n", addr, count, channel->start,
              channel->length);
    return RET_INVALID_ADDR;
  }

  /*读取数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_default_read_bit_from_channel(channel, addr + i, buff, i);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_read_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                             uint8_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_READ_BITS, RET_BAD_PARAMS);
  return_value_if_fail(m->bits != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_read_bits_from_channel(m->bits, addr, count, buff);
}

static ret_t modbus_memory_default_read_input_bits(modbus_memory_t* memory, uint16_t addr,
                                                   uint16_t count, uint8_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_READ_BITS, RET_BAD_PARAMS);
  return_value_if_fail(m->bits != NULL, RET_BAD_PARAMS);
  return_value_if_fail(m->input_bits != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_read_bits_from_channel(m->input_bits, addr, count, buff);
}

static ret_t modbus_memory_default_read_register_from_channel(modbus_server_channel_t* channel,
                                                              uint16_t addr, uint16_t* buff) {
  uint16_t offset = addr - channel->start;
  uint16_t* data = (uint16_t*)channel->data;

  ENSURE(offset < channel->length);
  *buff = int16_to_big_endian(data[offset]);

  return RET_OK;
}

static ret_t modbus_memory_default_read_registers_from_channel(modbus_server_channel_t* channel,
                                                               uint16_t addr, uint16_t count,
                                                               uint16_t* buff) {
  uint16_t i = 0;
  /*检测地址和范围是否合法*/
  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("invalid addr: addr:%d, count:%d, start:%d, length:%d\n", addr, count, channel->start,
              channel->length);
    return RET_INVALID_ADDR;
  }

  /*准备数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_default_read_register_from_channel(channel, addr + i, buff + i);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_read_registers(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t count, uint16_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_READ_REGISTERS, RET_BAD_PARAMS);
  return_value_if_fail(m->registers != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_read_registers_from_channel(m->registers, addr, count, buff);
}

static ret_t modbus_memory_default_read_input_registers(modbus_memory_t* memory, uint16_t addr,
                                                        uint16_t count, uint16_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(m->input_registers != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_read_registers_from_channel(m->input_registers, addr, count, buff);
}

static ret_t modbus_memory_default_write_bit_to_channel(modbus_server_channel_t* channel,
                                                        uint16_t addr, uint8_t value) {
  uint16_t offset = addr - channel->start;
  uint8_t* data = channel->data;
  uint32_t bytes = offset / 8 + 1;

  /*检测地址是否合法*/
  if (addr < channel->start || addr >= (channel->start + channel->length)) {
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  return bits_stream_set(data, bytes, offset, value);
}

static ret_t modbus_memory_default_write_bits_to_channel(modbus_server_channel_t* channel,
                                                         uint16_t addr, uint16_t count,
                                                         const uint8_t* buff) {
  uint16_t i = 0;
  bool_t value = FALSE;
  uint16_t bytes = (count + 7) / 8;

  /*检测地址和范围是否合法*/
  if (addr < channel->start || (count + addr) > (channel->start + channel->length)) {
    log_debug("invalid addr: addr:%d, count:%d, start:%d, length:%d\n", addr, count, channel->start,
              channel->length);
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  for (i = 0; i < count; i++) {
    ENSURE(bits_stream_get(buff, bytes, i, &value) == RET_OK);
    modbus_memory_default_write_bit_to_channel(channel, addr + i, value);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_write_bit(modbus_memory_t* memory, uint16_t addr,
                                             uint8_t value) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_write_bit_to_channel(m->bits, addr, value);
}

static ret_t modbus_memory_default_write_bits(modbus_memory_t* memory, uint16_t addr,
                                              uint16_t count, const uint8_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_WRITE_BITS, RET_BAD_PARAMS);
  return_value_if_fail(m->bits != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_write_bits_to_channel(m->bits, addr, count, buff);
}

static ret_t modbus_memory_default_write_register_to_channel(modbus_server_channel_t* channel,
                                                             uint16_t addr, uint16_t value) {
  uint16_t offset = addr - channel->start;
  uint16_t* data = (uint16_t*)channel->data;

  /*检测地址是否合法*/
  if (addr < channel->start || addr >= (channel->start + channel->length)) {
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  data[offset] = int16_from_big_endian(value);

  return RET_OK;
}

static ret_t modbus_memory_default_write_register(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t value) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  return modbus_memory_default_write_register_to_channel(m->registers, addr, value);
}

static ret_t modbus_memory_default_write_registers(modbus_memory_t* memory, uint16_t addr,
                                                   uint16_t count, const uint16_t* buff) {
  uint32_t i = 0;
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);
  return_value_if_fail(buff != NULL, RET_BAD_PARAMS);
  return_value_if_fail(count > 0, RET_BAD_PARAMS);
  return_value_if_fail(count <= MODBUS_MAX_WRITE_REGISTERS, RET_BAD_PARAMS);
  return_value_if_fail(m->registers != NULL, RET_BAD_PARAMS);

  if (addr < m->registers->start || (count + addr) > (m->registers->start + m->registers->length)) {
    log_debug("invalid addr: addr:%d, count:%d, start:%d, length:%d\n", addr, count,
              m->registers->start, m->registers->length);
    return RET_INVALID_ADDR;
  }

  for (i = 0; i < count; i++) {
    modbus_memory_default_write_register_to_channel(m->registers, addr + i, buff[i]);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_destroy(modbus_memory_t* memory) {
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL, RET_BAD_PARAMS);

  modbus_server_channel_destroy(memory_default->bits);
  modbus_server_channel_destroy(memory_default->input_bits);
  modbus_server_channel_destroy(memory_default->registers);
  modbus_server_channel_destroy(memory_default->input_registers);
  
  TKMEM_FREE(memory_default);

  return RET_OK;
}

modbus_memory_t* modbus_memory_default_create_test(void) {
  modbus_server_channel_t* bits = modbus_server_channel_create(
      MODBUS_SERVER_CHANNEL_BITS, MODBUS_DEMO_BITS_ADDRESS, MODBUS_DEMO_BITS_NB, TRUE);
  modbus_server_channel_t* input_bits =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_INPUT_BITS, MODBUS_DEMO_INPUT_BITS_ADDRESS,
                                   MODBUS_DEMO_INPUT_BITS_NB, FALSE);
  modbus_server_channel_t* registers =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_REGISTERS, MODBUS_DEMO_REGISTERS_ADDRESS,
                                   MODBUS_DEMO_REGISTERS_NB, TRUE);
  modbus_server_channel_t* input_registers = modbus_server_channel_create(
      MODBUS_SERVER_CHANNEL_INPUT_REGISTERS, MODBUS_DEMO_INPUT_REGISTERS_ADDRESS,
      MODBUS_DEMO_INPUT_REGISTERS_NB, FALSE);

  return modbus_memory_default_create(bits, input_bits, registers, input_registers);
}

modbus_memory_t* modbus_memory_default_create(modbus_server_channel_t* bits,
                                              modbus_server_channel_t* input_bits,
                                              modbus_server_channel_t* registers,
                                              modbus_server_channel_t* input_registers) {
  modbus_memory_default_t* memory = TKMEM_ZALLOC(modbus_memory_default_t);
  return_value_if_fail(memory != NULL, NULL);

  memory->memory.read_bits = modbus_memory_default_read_bits;
  memory->memory.read_input_bits = modbus_memory_default_read_input_bits;
  memory->memory.read_registers = modbus_memory_default_read_registers;
  memory->memory.read_input_registers = modbus_memory_default_read_input_registers;
  memory->memory.write_bit = modbus_memory_default_write_bit;
  memory->memory.write_bits = modbus_memory_default_write_bits;
  memory->memory.write_register = modbus_memory_default_write_register;
  memory->memory.write_registers = modbus_memory_default_write_registers;
  memory->memory.destroy = modbus_memory_default_destroy;

  memory->bits = bits;
  memory->input_bits = input_bits;
  memory->registers = registers;
  memory->input_registers = input_registers;

  return (modbus_memory_t*)memory;
}

modbus_memory_t* modbus_memory_default_create_with_conf(conf_node_t* node) {
  modbus_server_channel_t* bits = NULL;
  modbus_server_channel_t* input_bits = NULL;
  modbus_server_channel_t* registers = NULL;
  modbus_server_channel_t* input_registers = NULL;
  conf_node_t* iter = conf_node_get_first_child(node);
  return_value_if_fail(iter != NULL, NULL);

  while (iter != NULL) {
    modbus_server_channel_t* channel = modbus_server_channel_create_with_conf(iter);

    if (channel != NULL) {
      if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_BITS)) {
        bits = channel;
      } else if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_INPUT_BITS)) {
        input_bits = channel;
      } else if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_REGISTERS)) {
        registers = channel;
      } else if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_INPUT_REGISTERS)) {
        input_registers = channel;
      } else {
        modbus_server_channel_destroy(channel);
        log_debug("invalid channel name: %s\n", channel->name);
      }
    }

    iter = iter->next;
  }

  return modbus_memory_default_create(bits, input_bits, registers, input_registers);
}

modbus_memory_default_t* modbus_memory_default_cast(modbus_memory_t* memory) {
  return_value_if_fail(memory != NULL, NULL);

  if (memory->read_bits == modbus_memory_default_read_bits) {
    return (modbus_memory_default_t*)memory;
  }

  return NULL;
}

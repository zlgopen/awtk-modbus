/**
 * File:   modbus_memory_default.c
 * Author: AWTK Develop Team
 * Brief:  modbus memory default
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
 * 2023-10-02 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "modbus_memory_default.h"

typedef struct _bits_memory_t {
  uint32_t start;
  uint32_t count;
  uint8_t* data;
} bits_memory_t;

static ret_t modbus_memory_read_bits_impl(bits_memory_t* memory, uint16_t addr, uint16_t count,
                                          uint8_t* buff) {
  uint32_t i = 0;
  uint8_t* p = NULL;
  uint16_t offset = 0;
  uint16_t bytes = modbus_bits_to_bytes(count);

  if (count < 1 || count > MODBUS_MAX_READ_BITS) {
    return RET_EXCEED_RANGE;
  }

  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);
  if (addr < memory->start || (addr + count) > (memory->start + memory->count)) {
    return RET_INVALID_ADDR;
  }

  offset = addr - memory->start;
  p = memory->data + offset;

  for (i = 0; i < count; i++) {
    bits_stream_set(buff, bytes, i, p[i]);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_read_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                             uint8_t* buff) {
  bits_memory_t bits_memory;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL && buff != NULL, RET_BAD_PARAMS);

  bits_memory.start = memory_default->bits_start;
  bits_memory.count = memory_default->bits_count;
  bits_memory.data = memory_default->bits_data;

  return modbus_memory_read_bits_impl(&bits_memory, addr, count, buff);
}

static ret_t modbus_memory_default_read_input_bits(modbus_memory_t* memory, uint16_t addr,
                                                   uint16_t count, uint8_t* buff) {
  bits_memory_t bits_memory;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL && buff != NULL, RET_BAD_PARAMS);

  bits_memory.start = memory_default->input_bits_start;
  bits_memory.count = memory_default->input_bits_count;
  bits_memory.data = memory_default->input_bits_data;

  return modbus_memory_read_bits_impl(&bits_memory, addr, count, buff);
}

typedef struct _register_memory_t {
  uint32_t start;
  uint32_t count;
  uint16_t* data;
} register_memory_t;

static ret_t modbus_memory_read_registers_impl(register_memory_t* memory, uint16_t addr,
                                               uint16_t count, uint16_t* buff) {
  uint32_t i = 0;
  uint16_t* p = NULL;
  uint16_t offset = 0;
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (count < 1 || count > MODBUS_MAX_READ_REGISTERS) {
    return RET_EXCEED_RANGE;
  }

  if (addr < memory->start || (addr + count) > (memory->start + memory->count)) {
    return RET_INVALID_ADDR;
  }

  offset = addr - memory->start;
  p = memory->data + offset;

  for (i = 0; i < count; i++) {
    buff[i] = htons(p[i]);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_read_registers(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t count, uint16_t* buff) {
  register_memory_t register_memory;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL && buff != NULL, RET_BAD_PARAMS);

  register_memory.start = memory_default->registers_start;
  register_memory.count = memory_default->registers_count;
  register_memory.data = memory_default->registers_data;

  return modbus_memory_read_registers_impl(&register_memory, addr, count, buff);
}

static ret_t modbus_memory_default_read_input_registers(modbus_memory_t* memory, uint16_t addr,
                                                        uint16_t count, uint16_t* buff) {
  register_memory_t register_memory;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL && buff != NULL, RET_BAD_PARAMS);

  register_memory.start = memory_default->input_registers_start;
  register_memory.count = memory_default->input_registers_count;
  register_memory.data = memory_default->input_registers_data;

  return modbus_memory_read_registers_impl(&register_memory, addr, count, buff);
}

static ret_t modbus_memory_write_bits_impl(bits_memory_t* memory, uint16_t addr, uint16_t count,
                                           const uint8_t* buff) {
  uint32_t i = 0;
  uint8_t* p = NULL;
  uint16_t offset = 0;
  uint16_t bytes = modbus_bits_to_bytes(count);
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (count < 1 || count > MODBUS_MAX_WRITE_BITS) {
    return RET_EXCEED_RANGE;
  }

  if (addr < memory->start || (addr + count) > (memory->start + memory->count)) {
    return RET_INVALID_ADDR;
  }

  offset = addr - memory->start;
  p = memory->data + offset;

  for (i = 0; i < count; i++) {
    bool_t v = FALSE;
    bits_stream_get(buff, bytes, i, &v);
    p[i] = v;
  }

  return RET_OK;
}

static ret_t modbus_memory_default_write_bit(modbus_memory_t* memory, uint16_t addr,
                                             uint8_t value) {
  return modbus_memory_write_bits(memory, addr, 1, &value);
}

static ret_t modbus_memory_default_write_bits(modbus_memory_t* memory, uint16_t addr,
                                              uint16_t count, const uint8_t* buff) {
  bits_memory_t bits_memory;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL && buff != NULL, RET_BAD_PARAMS);

  bits_memory.start = memory_default->bits_start;
  bits_memory.count = memory_default->bits_count;
  bits_memory.data = memory_default->bits_data;

  return modbus_memory_write_bits_impl(&bits_memory, addr, count, buff);
}

static ret_t modbus_memory_default_write_register(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t value) {
  return modbus_memory_write_registers(memory, addr, 1, &value);
}

static ret_t modbus_memory_write_registers_impl(register_memory_t* memory, uint16_t addr,
                                                uint16_t count, const uint16_t* buff) {
  uint32_t i = 0;
  uint16_t* p = NULL;
  uint16_t offset = 0;
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (count < 1 || count > MODBUS_MAX_WRITE_REGISTERS) {
    return RET_EXCEED_RANGE;
  }

  if (addr < memory->start || (addr + count) > (memory->start + memory->count)) {
    return RET_INVALID_ADDR;
  }

  offset = addr - memory->start;
  p = memory->data + offset;

  for (i = 0; i < count; i++) {
    p[i] = ntohs(buff[i]);
  }

  return RET_OK;
}

static ret_t modbus_memory_default_write_registers(modbus_memory_t* memory, uint16_t addr,
                                                   uint16_t count, const uint16_t* buff) {
  register_memory_t register_memory;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL && buff != NULL, RET_BAD_PARAMS);

  register_memory.start = memory_default->registers_start;
  register_memory.count = memory_default->registers_count;
  register_memory.data = memory_default->registers_data;

  return modbus_memory_write_registers_impl(&register_memory, addr, count, buff);
}

static ret_t modbus_memory_default_destroy(modbus_memory_t* memory) {
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL, RET_BAD_PARAMS);

  if (memory_default->bits_data != NULL) {
    TKMEM_FREE(memory_default->bits_data);
  }
  if (memory_default->input_bits_data != NULL) {
    TKMEM_FREE(memory_default->input_bits_data);
  }
  if (memory_default->registers_data != NULL) {
    TKMEM_FREE(memory_default->registers_data);
  }
  if (memory_default->input_registers_data != NULL) {
    TKMEM_FREE(memory_default->input_registers_data);
  }
  TKMEM_FREE(memory_default);

  return RET_OK;
}

modbus_memory_t* modbus_memory_default_create(uint32_t bits_start, uint32_t bits_count,
                                              uint32_t input_bits_start, uint32_t input_bits_count,
                                              uint32_t registers_start, uint32_t registers_count,
                                              uint32_t input_registers_start,
                                              uint32_t input_registers_count) {
  modbus_memory_default_t* memory = TKMEM_ZALLOC(modbus_memory_default_t);
  return_value_if_fail(memory != NULL, NULL);

  memory->bits_start = bits_start;
  memory->bits_count = bits_count;
  memory->input_bits_start = input_bits_start;
  memory->input_bits_count = input_bits_count;
  memory->registers_start = registers_start;
  memory->registers_count = registers_count;
  memory->input_registers_start = input_registers_start;
  memory->input_registers_count = input_registers_count;

  memory->bits_data = TKMEM_ALLOC(bits_count);
  goto_error_if_fail(memory->bits_data != NULL);

  memory->input_bits_data = TKMEM_ALLOC(input_bits_count);
  goto_error_if_fail(memory->input_bits_data != NULL);

  memory->registers_data = TKMEM_ALLOC(registers_count * sizeof(uint16_t));
  goto_error_if_fail(memory->registers_data != NULL);

  memory->input_registers_data = TKMEM_ALLOC(input_registers_count * sizeof(uint16_t));
  goto_error_if_fail(memory->input_registers_data != NULL);

  memory->memory.read_bits = modbus_memory_default_read_bits;
  memory->memory.read_input_bits = modbus_memory_default_read_input_bits;
  memory->memory.read_registers = modbus_memory_default_read_registers;
  memory->memory.read_input_registers = modbus_memory_default_read_input_registers;
  memory->memory.write_bit = modbus_memory_default_write_bit;
  memory->memory.write_bits = modbus_memory_default_write_bits;
  memory->memory.write_register = modbus_memory_default_write_register;
  memory->memory.write_registers = modbus_memory_default_write_registers;
  memory->memory.destroy = modbus_memory_default_destroy;

  return (modbus_memory_t*)memory;

error:
  modbus_memory_destroy((modbus_memory_t*)memory);
  return NULL;
}

modbus_memory_default_t* modbus_memory_default_cast(modbus_memory_t* memory) {
  return_value_if_fail(memory != NULL, NULL);

  if (memory->read_bits == modbus_memory_default_read_bits) {
    return (modbus_memory_default_t*)memory;
  }

  return NULL;
}

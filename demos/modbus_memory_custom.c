/**
 * File:   modbus_memory_custom.c
 * Author: AWTK Develop Team
 * Brief:  modbus memory custom
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
 * 2023-10-02 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "modbus_memory_custom.h"

/*一个字节8个位表示8个LED*/
static uint8_t s_leds = 0;
#define MODBUS_CUSTOM_BITS_ADDRESS 0
#define MODBUS_CUSTOM_BITS_COUNT 8

static ret_t modbus_memory_custom_read_bit(modbus_memory_t* memory, uint16_t addr, uint8_t* buff,
                                           uint16_t offset) {
  bool_t value = FALSE;
  uint16_t bytes = offset / 8 + 1;
  uint16_t src_offset = addr - MODBUS_CUSTOM_BITS_ADDRESS;

  ENSURE(bits_stream_get(&s_leds, 1, src_offset, &value) == RET_OK);

  return bits_stream_set(buff, bytes, offset, value);
}

static ret_t modbus_memory_custom_read_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                            uint8_t* buff) {
  uint16_t i = 0;
  /*检测地址和范围是否合法*/
  if (addr < MODBUS_CUSTOM_BITS_ADDRESS ||
      (count + addr) > (MODBUS_CUSTOM_BITS_ADDRESS + MODBUS_CUSTOM_BITS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*读取数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_custom_read_bit(memory, addr + i, buff, i);
  }

  return RET_OK;
}

#define MODBUS_CUSTOM_INPUT_BITS_ADDRESS 0
#define MODBUS_CUSTOM_INPUT_BITS_COUNT 8

static ret_t modbus_memory_custom_read_input_bit(modbus_memory_t* memory, uint16_t addr,
                                                 uint8_t* buff, uint16_t offset) {
  bool_t value = FALSE;
  uint16_t bytes = offset / 8 + 1;
  uint16_t src_offset = addr - MODBUS_CUSTOM_BITS_ADDRESS;

  switch (src_offset) {
    case 0: /*开关0*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    case 1: /*开关1*/
    {
      value = FALSE; /*读取实际的值*/
      break;
    }
    case 2: /*开关2*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    case 3: /*开关3*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    case 4: /*开关4*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    case 5: /*开关5*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    case 6: /*开关6*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    case 7: /*开关7*/
    {
      value = TRUE; /*读取实际的值*/
      break;
    }
    default:
      break;
  }

  return bits_stream_set(buff, bytes, offset, value);
}

static ret_t modbus_memory_custom_read_input_bits(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t count, uint8_t* buff) {
  uint16_t i = 0;

  /*检测地址和范围是否合法*/
  if (addr < MODBUS_CUSTOM_INPUT_BITS_ADDRESS ||
      (count + addr) > (MODBUS_CUSTOM_INPUT_BITS_ADDRESS + MODBUS_CUSTOM_INPUT_BITS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*准备数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_custom_read_input_bit(memory, addr + i, buff, i);
  }

  return RET_OK;
}

#define MODBUS_CUSTOM_REGISTERS_ADDRESS 0
#define MODBUS_CUSTOM_REGISTERS_COUNT 5

static uint16_t s_position = 0;     /*位置*/
static uint16_t s_velocity = 0;     /*速度*/
static uint16_t s_acceleration = 0; /*加速度*/
static uint16_t s_jerk = 0;         /*加加速度*/
static uint16_t s_torque = 0;       /*力矩*/

static ret_t modbus_memory_custom_read_register(modbus_memory_t* memory, uint16_t addr,
                                                uint16_t* buff) {
  uint16_t value = 0;
  uint16_t offset = addr - MODBUS_CUSTOM_REGISTERS_ADDRESS;

  switch (offset) {
    case 0: {
      value = s_position;
      break;
    }
    case 1: {
      value = s_velocity;
      break;
    }
    case 2: {
      value = s_acceleration;
      break;
    }
    case 3: {
      value = s_jerk;
      break;
    }
    case 4: {
      value = s_torque;
      break;
    }
  }

  *buff = int16_to_big_endian(value);

  return RET_OK;
}

static ret_t modbus_memory_custom_read_registers(modbus_memory_t* memory, uint16_t addr,
                                                 uint16_t count, uint16_t* buff) {
  uint16_t i = 0;
  /*检测地址和范围是否合法*/
  if (addr < MODBUS_CUSTOM_REGISTERS_ADDRESS ||
      (count + addr) > (MODBUS_CUSTOM_REGISTERS_ADDRESS + MODBUS_CUSTOM_REGISTERS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*准备数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_custom_read_register(memory, addr + i, buff + i);
  }

  return RET_OK;
}

#define MODBUS_CUSTOM_INPUT_REGISTERS_ADDRESS 0
#define MODBUS_CUSTOM_INPUT_REGISTERS_COUNT 4

static ret_t modbus_memory_custom_read_input_register(modbus_memory_t* memory, uint16_t addr,
                                                      uint16_t* buff) {
  uint16_t value = 0;
  /*准备数据*/
  switch (addr) {
    case 0x00: /*NO*/ {
      value = 1000; /*读取实际的值*/
      break;
    }
    case 0x01: /*NO2*/ {
      value = 2000; /*读取实际的值*/
      break;
    }
    case 0x02: /*SO2*/ {
      value = 3000; /*读取实际的值*/
      break;
    }
    case 0x03: /*SO3*/ {
      value = 4000; /*读取实际的值*/
      break;
    }
    default:
      break;
  }

  *buff = int16_to_big_endian(value);

  return RET_OK;
}

static ret_t modbus_memory_custom_read_input_registers(modbus_memory_t* memory, uint16_t addr,
                                                       uint16_t count, uint16_t* buff) {
  uint16_t i = 0;
  /*检测地址和范围是否合法*/
  if (addr < MODBUS_CUSTOM_INPUT_REGISTERS_ADDRESS ||
      (count + addr) >
          (MODBUS_CUSTOM_INPUT_REGISTERS_ADDRESS + MODBUS_CUSTOM_INPUT_REGISTERS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*准备数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_custom_read_input_register(memory, addr + i, buff + i);
  }

  return RET_OK;
}

static ret_t modbus_memory_custom_write_bit(modbus_memory_t* memory, uint16_t addr, uint8_t value) {
  uint16_t offset = addr - MODBUS_CUSTOM_BITS_ADDRESS;

  /*检测地址是否合法*/
  if (addr < MODBUS_CUSTOM_BITS_ADDRESS ||
      addr >= (MODBUS_CUSTOM_BITS_ADDRESS + MODBUS_CUSTOM_BITS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  return bits_stream_set(&s_leds, 1, offset, value);
}

static ret_t modbus_memory_custom_write_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                             const uint8_t* buff) {
  uint16_t i = 0;
  bool_t value = FALSE;
  uint16_t bytes = (count + 7) / 8;

  /*检测地址和范围是否合法*/
  if (addr < MODBUS_CUSTOM_BITS_ADDRESS ||
      (count + addr) > (MODBUS_CUSTOM_BITS_ADDRESS + MODBUS_CUSTOM_BITS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  for (i = 0; i < count; i++) {
    ENSURE(bits_stream_get(buff, bytes, i, &value) == RET_OK);
    modbus_memory_custom_write_bit(memory, addr + i, value);
  }

  return RET_OK;
}

static ret_t modbus_memory_custom_write_register(modbus_memory_t* memory, uint16_t addr,
                                                 uint16_t value) {
  uint16_t offset = addr - MODBUS_CUSTOM_REGISTERS_ADDRESS;

  /*检测地址是否合法*/
  if (addr < MODBUS_CUSTOM_REGISTERS_ADDRESS ||
      addr >= (MODBUS_CUSTOM_REGISTERS_ADDRESS + MODBUS_CUSTOM_REGISTERS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  value = int16_from_big_endian(value);
  switch (offset) {
    case 0: {
      s_position = value;
      break;
    }
    case 1: {
      s_velocity = value;
      break;
    }
    case 2: {
      s_acceleration = value;
      break;
    }
    case 3: {
      s_jerk = value;
      break;
    }
    case 4: {
      s_torque = value;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t modbus_memory_custom_write_registers(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t count, const uint16_t* buff) {
  uint16_t i = 0;

  /*检测地址和范围是否合法*/
  if (addr < MODBUS_CUSTOM_REGISTERS_ADDRESS ||
      (count + addr) > (MODBUS_CUSTOM_REGISTERS_ADDRESS + MODBUS_CUSTOM_REGISTERS_COUNT)) {
    return RET_INVALID_ADDR;
  }

  /*写入数据*/
  for (i = 0; i < count; i++) {
    modbus_memory_custom_write_register(memory, addr + i, buff[i]);
  }

  return RET_OK;
}

static ret_t modbus_memory_custom_destroy(modbus_memory_t* memory) {
  modbus_memory_custom_t* memory_custom = MODBUS_MEMORY_CUSTOM(memory);
  return_value_if_fail(memory_custom != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(memory_custom);

  return RET_OK;
}

modbus_memory_t* modbus_memory_custom_create(void) {
  modbus_memory_custom_t* memory = TKMEM_ZALLOC(modbus_memory_custom_t);
  return_value_if_fail(memory != NULL, NULL);

  memory->memory.read_bits = modbus_memory_custom_read_bits;
  memory->memory.read_input_bits = modbus_memory_custom_read_input_bits;
  memory->memory.read_registers = modbus_memory_custom_read_registers;
  memory->memory.read_input_registers = modbus_memory_custom_read_input_registers;
  memory->memory.write_bit = modbus_memory_custom_write_bit;
  memory->memory.write_bits = modbus_memory_custom_write_bits;
  memory->memory.write_register = modbus_memory_custom_write_register;
  memory->memory.write_registers = modbus_memory_custom_write_registers;
  memory->memory.destroy = modbus_memory_custom_destroy;

  return (modbus_memory_t*)memory;
}

modbus_memory_custom_t* modbus_memory_custom_cast(modbus_memory_t* memory) {
  return_value_if_fail(memory != NULL, NULL);

  if (memory->read_bits == modbus_memory_custom_read_bits) {
    return (modbus_memory_custom_t*)memory;
  }

  return NULL;
}

/**
 * File:   modbus_memory.c
 * Author: AWTK Develop Team
 * Brief:  modbus memory
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

#include "modbus_memory.h"

ret_t modbus_memory_read_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                              uint8_t* buff) {
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (memory->read_bits != NULL) {
    return memory->read_bits(memory, addr, count, buff);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_read_input_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                    uint8_t* buff) {
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (memory->read_input_bits != NULL) {
    return memory->read_input_bits(memory, addr, count, buff);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_read_registers(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                   uint16_t* buff) {
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (memory->read_registers != NULL) {
    return memory->read_registers(memory, addr, count, buff);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_read_input_registers(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                         uint16_t* buff) {
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (memory->read_input_registers != NULL) {
    return memory->read_input_registers(memory, addr, count, buff);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_write_bit(modbus_memory_t* memory, uint16_t addr, uint8_t value) {
  return_value_if_fail(memory != NULL, RET_BAD_PARAMS);

  if (memory->write_bit != NULL) {
    return memory->write_bit(memory, addr, value);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_write_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                               const uint8_t* buff) {
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (memory->write_bits != NULL) {
    return memory->write_bits(memory, addr, count, buff);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_write_register(modbus_memory_t* memory, uint16_t addr, uint16_t value) {
  return_value_if_fail(memory != NULL, RET_BAD_PARAMS);

  if (memory->write_register != NULL) {
    return memory->write_register(memory, addr, value);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_write_registers(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                    const uint16_t* buff) {
  return_value_if_fail(memory != NULL && buff != NULL, RET_BAD_PARAMS);

  if (memory->write_registers != NULL) {
    return memory->write_registers(memory, addr, count, buff);
  }

  return RET_NOT_IMPL;
}

ret_t modbus_memory_destroy(modbus_memory_t* memory) {
  return_value_if_fail(memory != NULL, RET_BAD_PARAMS);

  if (memory->destroy != NULL) {
    return memory->destroy(memory);
  }

  return RET_OK;
}

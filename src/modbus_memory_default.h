/**
 * File:   modbus_memory_default.h
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

#ifndef TK_MODBUS_MEMORY_DEFAULT_H
#define TK_MODBUS_MEMORY_DEFAULT_H

#include "modbus_types_def.h"
#include "modbus_memory.h"

BEGIN_C_DECLS

/**
 * @class modbus_memory_default_t
 * 
 * modbus的默认实现。
 */
typedef struct _modbus_memory_default_t {
  modbus_memory_t memory;

  /**
   * @property {uint32_t} bits_start
   * 位起始地址。
   */
  uint32_t bits_start;
  /**
   * @property {uint32_t} bits_count
   * 位数量。
   */
  uint32_t bits_count;
  /**
   * @property {uint8_t*} bits_data
   * 位数据。
   */
  uint8_t* bits_data;

  /**
   * @property {uint32_t} input_bits_start
   * 输入位起始地址。
   */
  uint32_t input_bits_start;
  /**
   * @property {uint32_t} input_bits_count
   * 输入位数量。
   */
  uint32_t input_bits_count;
  /**
   * @property {uint8_t*} input_bits_data
   * 输入位数据。
   */
  uint8_t* input_bits_data;
  /**
   * @property {uint32_t} registers_start
   * 寄存器起始地址。
   */
  uint32_t registers_start;
  /**
   * @property {uint32_t} registers_count
   * 寄存器数量。
   */
  uint32_t registers_count;
  /**
   * @property {uint16_t*} registers_data
   * 寄存器数据。
   */
  uint16_t* registers_data;

  /**
   * @property {uint32_t} input_registers_start
   * 输入寄存器起始地址。
   */
  uint32_t input_registers_start;
  /**
   * @property {uint32_t} input_registers_count
   * 输入寄存器数量。
   */
  uint32_t input_registers_count;
  /**
   * @property {uint16_t*} input_registers_data
   * 输入寄存器数据。
   */
  uint16_t* input_registers_data;
} modbus_memory_default_t;

/**
 * @method modbus_memory_default_create
 * 创建默认memory。
 * @param {uint32_t} bits_start 位起始地址。
 * @param {uint32_t} bits_count 位数量。
 * @param {uint32_t} input_bits_start 输入位起始地址。
 * @param {uint32_t} input_bits_count 输入位数量。
 * @param {uint32_t} registers_start 寄存器起始地址。
 * @param {uint32_t} registers_count 寄存器数量。
 * @param {uint32_t} input_registers_start 输入寄存器起始地址。
 * @param {uint32_t} input_registers_count 输入寄存器数量。
 * 
 * @return {modbus_memory_t*} 返回modbus memory对象。
 */
modbus_memory_t* modbus_memory_default_create(uint32_t bits_start, uint32_t bits_count,
                                              uint32_t input_bits_start, uint32_t input_bits_count,
                                              uint32_t registers_start, uint32_t registers_count,
                                              uint32_t input_registers_start,
                                              uint32_t input_registers_count);

/**
 * @method modbus_memory_default_cast
 * 转换为modbus_memory_default_t。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * 
 * @return {modbus_memory_default_t*} 返回modbus_memory_default_t对象。
 */
modbus_memory_default_t* modbus_memory_default_cast(modbus_memory_t* memory);

#define MODBUS_MEMORY_DEFAULT(memory) modbus_memory_default_cast((modbus_memory_t*)memory)
END_C_DECLS

#endif /*TK_MODBUS_MEMORY_DEFAULT_H*/

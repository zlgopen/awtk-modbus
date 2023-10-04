/**
 * File:   modbus_memory.h
 * Author: AWTK Develop Team
 * Brief:  modbus memory
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

#ifndef TK_MODBUS_MEMORY_H
#define TK_MODBUS_MEMORY_H

#include "modbus_types_def.h"

BEGIN_C_DECLS

/**
 * @class modbus_memory_t
 * 
 * modbus memory
 */
typedef struct _modbus_memory_t {
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
} modbus_memory_t;

/**
 * @method modbus_memory_create
 * 创建modbus memory。
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
modbus_memory_t* modbus_memory_create(uint32_t bits_start, uint32_t bits_count,
                                      uint32_t input_bits_start, uint32_t input_bits_count,
                                      uint32_t registers_start, uint32_t registers_count,
                                      uint32_t input_registers_start,
                                      uint32_t input_registers_count);

/**
 * @method modbus_memory_read_bits
 * 读取bits。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint8_t*} buff 读取的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_read_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                              uint8_t* buff);
/**
 * @method modbus_memory_read_input_bits
 * 读取input bits。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint8_t*} buff 读取的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_read_input_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                    uint8_t* buff);

/**
 * @method modbus_memory_read_registers
 * 读取registers。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint16_t*} buff 读取的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_read_registers(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                   uint16_t* buff);

/**
 * @method modbus_memory_read_input_registers
 * 读取input registers。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint16_t*} buff 读取的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_read_input_registers(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                         uint16_t* buff);

/**
 * @method modbus_memory_write_bit
 * 写入bit。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint8_t} value 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_write_bit(modbus_memory_t* memory, uint16_t addr, uint8_t value);

/**
 * @method modbus_memory_write_register
 * 写入register。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} value 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_write_register(modbus_memory_t* memory, uint16_t addr, uint16_t value);

/**
 * @method modbus_memory_write_bits
 * 写入bits。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {const uint8_t*} buff 写入的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_write_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                               const uint8_t* buff);

/**
 * @method modbus_memory_write_registers
 * 写入registers。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {const uint16_t*} buff 写入的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_write_registers(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                    const uint16_t* buff);

/**
 * @method modbus_memory_destroy
 * 销毁modbus memory。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_memory_destroy(modbus_memory_t* memory);

END_C_DECLS

#endif /*TK_MODBUS_MEMORY_H*/

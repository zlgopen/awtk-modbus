/**
 * File:   modbus_memory.h
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

#ifndef TK_MODBUS_MEMORY_H
#define TK_MODBUS_MEMORY_H

#include "modbus_types_def.h"

BEGIN_C_DECLS

struct _modbus_memory_t;
typedef struct _modbus_memory_t modbus_memory_t;

typedef ret_t (*modbus_memory_read_bits_t)(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                           uint8_t* buff);
typedef ret_t (*modbus_memory_read_input_bits_t)(modbus_memory_t* memory, uint16_t addr,
                                                 uint16_t count, uint8_t* buff);
typedef ret_t (*modbus_memory_read_registers_t)(modbus_memory_t* memory, uint16_t addr,
                                                uint16_t count, uint16_t* buff);
typedef ret_t (*modbus_memory_read_input_registers_t)(modbus_memory_t* memory, uint16_t addr,
                                                      uint16_t count, uint16_t* buff);
typedef ret_t (*modbus_memory_write_bit_t)(modbus_memory_t* memory, uint16_t addr, uint8_t value);
typedef ret_t (*modbus_memory_write_register_t)(modbus_memory_t* memory, uint16_t addr,
                                                uint16_t value);
typedef ret_t (*modbus_memory_write_bits_t)(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                            const uint8_t* buff);
typedef ret_t (*modbus_memory_write_registers_t)(modbus_memory_t* memory, uint16_t addr,
                                                 uint16_t count, const uint16_t* buff);
typedef ret_t (*modbus_memory_destroy_t)(modbus_memory_t* memory);

/**
 * @class modbus_memory_t
 * 
 * modbus memory接口。
 */
typedef struct _modbus_memory_t {
  modbus_memory_read_bits_t read_bits;
  modbus_memory_read_input_bits_t read_input_bits;
  modbus_memory_read_registers_t read_registers;
  modbus_memory_read_input_registers_t read_input_registers;
  modbus_memory_write_bit_t write_bit;
  modbus_memory_write_register_t write_register;
  modbus_memory_write_bits_t write_bits;
  modbus_memory_write_registers_t write_registers;
  modbus_memory_destroy_t destroy;
} modbus_memory_t;

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

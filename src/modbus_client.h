/**
 * File:   modbus_client.h
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
 * 2023-09-29 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_MODBUS_CLIENT_H
#define TK_MODBUS_CLIENT_H

#include "modbus_common.h"

BEGIN_C_DECLS

/**
 * @class modbus_client_t
 * 
 * modbus client
 *
 * 示例
 *
 *```c
 *  uint8_t read_bits_result[4];
 *  uint8_t write_bits[] = {TRUE, FALSE, TRUE, FALSE};
 *  uint16_t read_registers_result[4];
 *  uint16_t write_registers[] = {0x1122, 0x2233, 0x3344, 0x4455};
 *
 *  modbus_client_t* client = modbus_client_create("tcp://localhost:502");
 *
 *  ENSURE(modbus_client_write_bit(client, MODBUS_DEMO_BITS_ADDRESS, 1) == RET_OK);
 *  ENSURE(modbus_client_read_bits(client, MODBUS_DEMO_BITS_ADDRESS, 1, read_bits_result) == RET_OK);
 *  ENSURE(read_bits_result[0] == TRUE);
 *
 *  ENSURE(modbus_client_write_bits(client, MODBUS_DEMO_BITS_ADDRESS, 4, write_bits) == RET_OK);
 *  ENSURE(modbus_client_read_bits(client, MODBUS_DEMO_BITS_ADDRESS, 4, read_bits_result) == RET_OK);
 *  ENSURE(read_bits_result[0] == TRUE);
 *  ENSURE(read_bits_result[1] == FALSE);
 *  ENSURE(read_bits_result[2] == TRUE);
 *  ENSURE(read_bits_result[3] == FALSE);
 *
 *  ENSURE(modbus_client_write_register(client, MODBUS_DEMO_REGISTERS_ADDRESS, 0xaabb) == RET_OK);
 *  ENSURE(modbus_client_read_registers(client, MODBUS_DEMO_REGISTERS_ADDRESS, 1, read_registers_result) == RET_OK);
 *  ENSURE(read_registers_result[0] == 0xaabb);
 *
 *  ENSURE(modbus_client_write_registers(client, MODBUS_DEMO_REGISTERS_ADDRESS, 4, write_registers) == RET_OK);
 *  ENSURE(modbus_client_read_registers(client, MODBUS_DEMO_REGISTERS_ADDRESS, 4, read_registers_result) == RET_OK);
 *  ENSURE(read_registers_result[0] == 0x1122);
 *  ENSURE(read_registers_result[1] == 0x2233);
 *  ENSURE(read_registers_result[2] == 0x3344);
 *  ENSURE(read_registers_result[3] == 0x4455);
 *
 *  ENSURE(modbus_client_read_input_bits(client, MODBUS_DEMO_INPUT_BITS_ADDRESS, 1, read_bits_result) == RET_OK);
 *  ENSURE(modbus_client_read_input_registers(client, MODBUS_DEMO_INPUT_REGISTERS_ADDRESS, 1, read_registers_result) == RET_OK);
 *
 *  modbus_client_destroy(client);
 *```  
 */
typedef struct _modbus_client_t {
  modbus_common_t common;
} modbus_client_t;

/**
 * @method modbus_client_create
 * 创建modbus client。
 * 
 * 示例(TCP)
 *
 *```c
 *  modbus_client_t* client = modbus_client_create("tcp://localhost:502");
 *```
 * 示例(RTU)
 *
 *```c
 *  modbus_client_t* client = modbus_client_create("serial:///dev/ttys125");
 *  modbus_client_set_slave(client, 1);
 *```
 * @param {const char*} url modbus server的url。
 * @return {modbus_client_t*} 返回modbus client对象。
 */
modbus_client_t* modbus_client_create(const char* url);

/**
 * @method modbus_client_create_with_io
 * 创建modbus client。
 * @param {tk_iostream_t*} io io对象。
 * @param {modbus_proto_t} proto 协议。
 * @return {modbus_client_t*} 返回modbus client对象。
 */
modbus_client_t* modbus_client_create_with_io(tk_iostream_t* io, modbus_proto_t proto);

/**
 * @method modbus_client_read_bits
 * 读取bits。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint8_t*} buff 读取的数据(每个bit在buff中占据1个字节)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_read_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                              uint8_t* buff);
/**
 * @method modbus_client_read_input_bits
 * 读取input bits。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint8_t*} buff 读取的数据(每个bit在buff中占据1个字节)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_read_input_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                                    uint8_t* buff);

/**
 * @method modbus_client_read_registers
 * 读取registers。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint16_t*} buff 读取的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_read_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                   uint16_t* buff);

/**
 * @method modbus_client_read_input_registers
 * 读取input registers。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {uint16_t*} buff 读取的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_read_input_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                         uint16_t* buff);

/**
 * @method modbus_client_write_bit
 * 写入bit。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint8_t} value 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_write_bit(modbus_client_t* client, uint16_t addr, uint8_t value);

/**
 * @method modbus_client_write_register
 * 写入register。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} value 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_write_register(modbus_client_t* client, uint16_t addr, uint16_t value);

/**
 * @method modbus_client_write_bits
 * 写入bits。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {const uint8_t*} buff 写入的数据(每个bit在buff中占据1个字节)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_write_bits(modbus_client_t* client, uint16_t addr, uint16_t count,
                               const uint8_t* buff);

/**
 * @method modbus_client_write_registers
 * 写入registers。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {const uint16_t*} buff 写入的数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_write_registers(modbus_client_t* client, uint16_t addr, uint16_t count,
                                    const uint16_t* buff);

/**
 * @method modbus_client_set_slave
 * 设置slave。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint8_t} slave slave。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_set_slave(modbus_client_t* client, uint8_t slave);

/**
 * @method modbus_client_destroy
 * 销毁modbus client。
 * @param {modbus_client_t*} client modbus client对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_destroy(modbus_client_t* client);

END_C_DECLS

#endif /*TK_MODBUS_CLIENT_H*/

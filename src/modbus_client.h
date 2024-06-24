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

#include "service/client.h"
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
  tk_client_t client;
  modbus_common_t common;

  /**
   * @property {uint32_t} response_timeout
   * @annotation ["readable"]
   * 数据帧应答超时时间
  */
  uint32_t response_timeout;

  /**
   * @property {uint32_t} frame_gap_time
   * 帧间间隔时间。(单位：us)
   */
  uint32_t frame_gap_time;

  /**
   * @property {uint32_t} retry_times
   * 重试次数。
   */
  uint32_t retry_times;
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
 * 示例(RTU over TCP)
 *
 *```c
 *  modbus_client_t* client = modbus_client_create("rtu+tcp://localhost:502");
 *```
 * 示例(RTU)
 *
 *```c
 *  modbus_client_t* client = modbus_client_create("serial:///dev/ttys125");
 *  modbus_client_set_slave(client, 1);
 *```
 * 示例(RTU Windows)
 *
 *```c
 *  modbus_client_t* client = modbus_client_create("serial://COM5");
 *  modbus_client_set_slave(client, 1);
 *```
 *
 * 串口URL参数说明：
 * * baudrate {int} 波特率。常用值： 9600, 38400, 57600, 115200。(默认值：115200)
 * * stopbits {string} 停止位。可选值：1, 1.5, 2。(默认值：1)
 * * parity {string} 校验位。可选值：none, odd, even, mark, space。(默认值：none)
 * * flowcontrol {string} 流控。可选值：none, hardware, software。(默认值：none)
 * * bytesize {string} 数据位。可选值：5, 6, 7, 8 。(默认值：8) 
 * 
 * > 示例：
 * * serial://COM1?baudrate=115200
 * * serial://COM2?baudrate=115200&stopbits=1&parity=odd&flowcontrol=soft&bytesize=8
 * 
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
 * @method modbus_client_set_retry_times
 * 设置重试次数。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint32_t} retry_times 重试次数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_set_retry_times(modbus_client_t* client, uint32_t retry_times);

/**
 * @method modbus_client_set_response_timeout
 * 设置数据帧应答超时时间。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint32_t} response_timeout 数据帧应答超时时间。(单位：us)
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_set_response_timeout(modbus_client_t* client, uint32_t response_timeout);

/**
 * @method modbus_client_set_frame_gap_time
 * 设置数据帧帧间隔时间。
 * @param {modbus_client_t*} client modbus client对象。
 * @param {uint32_t} response_timeout 数据帧应答超时时间。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_set_frame_gap_time(modbus_client_t* client, uint32_t frame_gap_time);

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

/**
 * File:   modbus_server_channel.h
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
 * 2024-03-10 Li XianJing <lixianjing@zlg.cn> created.
 *
 */

#include "tkc.h"
#include "conf_io/conf_node.h"
#include "modbus_types_def.h"

#ifndef MODBUS_SERVER_CHANNEL_H
#define MODBUS_SERVER_CHANNEL_H

BEGIN_C_DECLS

/**
 * @class modbus_server_channel_t
 * modbus_server_channel
 */
typedef struct _modbus_server_channel_t {
  /**
   * @property {char*} name
   * @annotation ["readable"]
   * 名称
  */
  char* name;

  /**
   * @property {uint32_t} start
   * @annotation ["readable"]
   * Offset
  */
  uint32_t start;

  /**
   * @property {uint32_t} length
   * @annotation ["readable"]
   * Length
  */
  uint32_t length;

  /**
   * @property {uint32_t} bytes
   * @annotation ["readable"]
   * bytes
  */
  uint32_t bytes;

  /**
   * @property {uint8_t*} data
   * @annotation ["readable"]
   * Data
  */
  uint8_t* data;

  /**
   * @property {bool_t} writable
   * @annotation ["readable"]
   * Writable
  */
  bool_t writable;
} modbus_server_channel_t;

/**
 * @method modbus_server_channel_create_with_conf
 * 创建modbus_server_channel对象。
 * @param {conf_node_t*} node 配置节点。
 *
 * @return {modbus_server_channel_t*} 返回对象。
 */
modbus_server_channel_t* modbus_server_channel_create_with_conf(conf_node_t* node);

/**
 * @method modbus_server_channel_create
 * 创建modbus_server_channel对象。
 *
 * @param {const char*} name 名称。
 * @param {uint32_t} start Offset。
 * @param {uint32_t} length Length。
 * @param {bool_t} writable 是否可写。
 *
 * @return {modbus_server_channel_t*} 返回对象。
 */
modbus_server_channel_t* modbus_server_channel_create(const char* name, uint32_t start,
                                                      uint32_t length, bool_t writable);

/**
 * @method modbus_server_channel_read_bits
 * 读取位数据。
 *
 * @param {modbus_server_channel_t*} channel 对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 数量。
 * @param {uint8_t*} buff 缓冲区。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_read_bits(modbus_server_channel_t* channel, uint16_t addr,
                                      uint16_t count, uint8_t* buff);

/**
 * @method modbus_server_channel_read_registers
 * 读取寄存器数据。
 * 
 * @param {modbus_server_channel_t*} channel 对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 数量。
 * @param {uint16_t*} buff 缓冲区。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_read_registers(modbus_server_channel_t* channel, uint16_t addr,
                                           uint16_t count, uint16_t* buff);

/**
 * @method modbus_server_channel_write_bits
 * 写入位数据。
 *
 * @param {modbus_server_channel_t*} channel 对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 数量。
 * @param {const uint8_t*} buff 缓冲区。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_write_bits(modbus_server_channel_t* channel, uint16_t addr,
                                       uint16_t count, const uint8_t* buff);

/**
 * @method modbus_server_channel_write_bit
 * 写入位数据。
 * 
 * @param {modbus_server_channel_t*} channel 对象。
 * @param {uint16_t} addr 地址。
 * @param {uint8_t} value 值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_write_bit(modbus_server_channel_t* channel, uint16_t addr,
                                      uint8_t value);

/**
 * @method modbus_server_channel_write_register
 * 
 * 写入寄存器数据。
 * 
 * @param {modbus_server_channel_t*} channel 对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} value 值。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_write_register(modbus_server_channel_t* channel, uint16_t addr,
                                           uint16_t value);

/**
 * @method modbus_server_channel_write_registers
 * 写入寄存器数据。
 * 
 * @param {modbus_server_channel_t*} channel 对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 数量。
 * @param {const uint16_t*} buff 缓冲区。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_write_registers(modbus_server_channel_t* channel, uint16_t addr,
                                            uint16_t count, const uint16_t* buff);

/**
 * @method modbus_server_channel_destroy
 * 销毁对象。
 *
 * @param {modbus_server_channel_t*} channel 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_server_channel_destroy(modbus_server_channel_t* channel);

#define MODBUS_SERVER_CHANNEL_BITS "bits"
#define MODBUS_SERVER_CHANNEL_REGISTERS "registers"
#define MODBUS_SERVER_CHANNEL_INPUT_BITS "input_bits"
#define MODBUS_SERVER_CHANNEL_INPUT_REGISTERS "input_registers"

END_C_DECLS

#endif /*MODBUS_SERVER_CHANNEL_H*/

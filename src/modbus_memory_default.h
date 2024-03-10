/**
 * File:   modbus_memory_default.h
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

#ifndef TK_MODBUS_MEMORY_DEFAULT_H
#define TK_MODBUS_MEMORY_DEFAULT_H

#include "conf_io/conf_node.h"
#include "modbus_types_def.h"
#include "modbus_memory.h"
#include "modbus_server_channel.h"

BEGIN_C_DECLS

/**
 * @class modbus_memory_default_t
 * 
 * 自定义的memory。
 */
typedef struct _modbus_memory_default_t {
  modbus_memory_t memory;

  /*private*/
  modbus_server_channel_t* bits;
  modbus_server_channel_t* input_bits;
  modbus_server_channel_t* registers;
  modbus_server_channel_t* input_registers;
} modbus_memory_default_t;

/**
 * @method modbus_memory_default_create
 * 创建modbus_memory_default_t对象。
 * @param {modbus_server_channel_t*} bits bits channel。
 * @param {modbus_server_channel_t*} input_bits input bits channel。
 * @param {modbus_server_channel_t*} registers registers channel。
 * @param {modbus_server_channel_t*} input_registers input registers channel。
 * 
 * @return {modbus_memory_t*} 返回modbus_memory_t对象。
 */
modbus_memory_t* modbus_memory_default_create(
  modbus_server_channel_t* bits,
  modbus_server_channel_t* input_bits,
  modbus_server_channel_t* registers,
  modbus_server_channel_t* input_registers);

/**
 * @method modbus_memory_default_create_test
 * 创建modbus_memory_default_t对象。
 * 
 * @return {modbus_memory_t*} 返回modbus_memory_t对象。
 */
modbus_memory_t* modbus_memory_default_create_test(void);

/**
 * @method modbus_memory_default_create_with_conf
 * 从配置文件创建modbus_memory_default_t对象。
 * @param {conf_node_t*} node 配置文件节点。
 * 
 * @return {modbus_memory_t*} 返回modbus_memory_t对象。
 */
modbus_memory_t* modbus_memory_default_create_with_conf(conf_node_t* node);

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

/**
 * File:   modbus_memory_custom.h
 * Author: AWTK Develop Team
 * Brief:  modbus memory custom
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

#ifndef TK_MODBUS_MEMORY_CUSTOM_H
#define TK_MODBUS_MEMORY_CUSTOM_H

#include "modbus_types_def.h"
#include "modbus_memory.h"

BEGIN_C_DECLS

/**
 * @class modbus_memory_custom_t
 * 
 * 自定义的memory。
 */
typedef struct _modbus_memory_custom_t {
  modbus_memory_t memory;
  /*private*/
} modbus_memory_custom_t;

/**
 * @method modbus_memory_custom_create
 * 创建自定义memory。
 * 
 * @return {modbus_memory_t*} 返回modbus memory对象。
 */
modbus_memory_t* modbus_memory_custom_create(void);

/**
 * @method modbus_memory_custom_cast
 * 转换为modbus_memory_custom_t。
 * @param {modbus_memory_t*} memory modbus memory对象。
 * 
 * @return {modbus_memory_custom_t*} 返回modbus_memory_custom_t对象。
 */
modbus_memory_custom_t* modbus_memory_custom_cast(modbus_memory_t* memory);

#define MODBUS_MEMORY_CUSTOM(memory) modbus_memory_custom_cast((modbus_memory_t*)memory)

END_C_DECLS

#endif /*TK_MODBUS_MEMORY_CUSTOM_H*/

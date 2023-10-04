/**
 * File:   modbus_service_rtu.h
 * Author: AWTK Develop Team
 * Brief:  modbus service rtu
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
 * 2023-10-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_MODBUS_SERVICE_RTU_H
#define TK_MODBUS_SERVICE_RTU_H

#include "modbus_service.h"

BEGIN_C_DECLS

/**
 * @class modbus_service_rtu_t
 * @annotation ["fake"]
 * modbus service rtu
 */

/**
 * @method modbus_service_rtu_start
 * 创建modbus service RTU。
 * @param {event_source_manager_t*} esm 事件管理对象(为NULL则阻塞运行)。
 * @param {modbus_memory_t*} memory 内存对象。
 * @param {modbus_hook_t*} hook hook对象。
 * @param {const char*} url URL。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_rtu_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               modbus_hook_t* hook, const char* url);

END_C_DECLS

#endif /*TK_MODBUS_SERVICE_RTU_H*/

/**
 * File:   modbus_service_tcp.h
 * Author: AWTK Develop Team
 * Brief:  modbus service tcp
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

#ifndef TK_MODBUS_SERVICE_TCP_H
#define TK_MODBUS_SERVICE_TCP_H

#include "modbus_service.h"

BEGIN_C_DECLS

/**
 * @class modbus_service_tcp_t
 * @annotation ["fake"]
 * modbus service tcp
 */

/**
 * @method modbus_service_tcp_start
 * 创建modbus service TCP。
 * @param {event_source_manager_t*} esm 事件管理对象(为NULL则阻塞运行)。
 * @param {modbus_memory_t*} memory 内存对象。
 * @param {modbus_hook_t*} hook hook对象。
 * @param {int} port 端口。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_tcp_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               modbus_hook_t* hook, int port);
END_C_DECLS

#endif /*TK_MODBUS_SERVICE_TCP_H*/

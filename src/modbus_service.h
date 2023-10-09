/**
 * File:   modbus_service.h
 * Author: AWTK Develop Team
 * Brief:  modbus service
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

#ifndef TK_MODBUS_SERVICE_H
#define TK_MODBUS_SERVICE_H

#include "modbus_common.h"
#include "modbus_memory.h"
#include "modbus_hook.h"

BEGIN_C_DECLS

/**
 * @class modbus_service_t
 * 
 * modbus service
 */
typedef struct _modbus_service_t {
  modbus_common_t common;
  modbus_memory_t* memory;
  modbus_hook_t* hook;
} modbus_service_t;

/**
 * @method modbus_service_create_with_io
 * 创建modbus service。
 * @param {tk_iostream_t*} io io对象。
 * @param {modbus_proto_t} proto 协议。
 * @param {modbus_memory_t*} memory memory对象。
 * @return {modbus_service_t*} 返回modbus service对象。
 */
modbus_service_t* modbus_service_create_with_io(tk_iostream_t* io, modbus_proto_t proto,
                                                modbus_memory_t* memory);

/**
 * @method modbus_service_set_slave
 * 设置slave。
 * @param {modbus_service_t*} service modbus service对象。
 * @param {uint8_t} slave slave。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_set_slave(modbus_service_t* service, uint8_t slave);

/**
 * @method modbus_service_set_hook
 * 设置hook。
 * @param {modbus_service_t*} service modbus service对象。
 * @param {modbus_hook_t*} hook hook对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_set_hook(modbus_service_t* service, modbus_hook_t* hook);

/**
 * @method modbus_service_dispatch
 * 分发请求。
 * @param {modbus_service_t*} service modbus service对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_dispatch(modbus_service_t* service);

/**
 * @method modbus_service_destroy
 * 销毁modbus service。
 * @param {modbus_service_t*} service modbus service对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_destroy(modbus_service_t* service);

/**
 * @method modbus_service_attach_to_event_source_manager
 * 关联到esm。
 * @param {modbus_service_t*} service modbus service对象。
 * @param {event_source_manager_t*} esm 事件管理器。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_attach_to_event_source_manager(modbus_service_t* service,
                                                    event_source_manager_t* esm);

/**
 * @method modbus_service_run
 * 阻塞运行。
 * @param {modbus_service_t*} service modbus service对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_run(modbus_service_t* service);

END_C_DECLS

#endif /*TK_MODBUS_SERVICE_H*/

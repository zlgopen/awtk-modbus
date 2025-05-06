/**
 * File:   modbus_service.h
 * Author: AWTK Develop Team
 * Brief:  modbus service
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

#ifndef TK_MODBUS_SERVICE_H
#define TK_MODBUS_SERVICE_H

#include "modbus_common.h"
#include "modbus_memory.h"
#include "service/service.h"

BEGIN_C_DECLS

struct _modbus_service_t;
typedef struct _modbus_service_t modbus_service_t;

typedef ret_t (*modbus_service_on_connected_t)(modbus_service_t* service, void* ctx);
typedef ret_t (*modbus_service_on_disconnected_t)(modbus_service_t* service, void* ctx);

typedef struct _modbus_service_args_t {
  modbus_proto_t proto;
  modbus_memory_t* memory;
  uint8_t slave;
  const wchar_t* ifname;
  void* ctx;
  modbus_service_on_connected_t on_connected;
} modbus_service_args_t;

/**
 * @class modbus_service_t
 * 
 * modbus service
 */
struct _modbus_service_t {
  tk_service_t service;
  modbus_common_t common;
  modbus_memory_t* memory;
  void* ctx;
  modbus_service_on_disconnected_t on_disconnected;
};

/**
 * @method modbus_service_create
 * 创建modbus service。
 * @param {tk_iostream_t*} io io对象。
 * @param {void*} args 参数。
 * @return {tk_service_t*} 返回modbus service对象。
 */
tk_service_t* modbus_service_create(tk_iostream_t* io, void* args);

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
 * @method modbus_service_dispatch
 * 分发请求。
 * @param {modbus_service_t*} service modbus service对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_dispatch(modbus_service_t* service);

/**
 * @method modbus_service_wait_for_data
 * 等待数据。
 * @param {modbus_service_t*} service modbus service对象。
 * @param {uint32_t} timeout 超时时间。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_wait_for_data(modbus_service_t* service, uint32_t timeout);

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

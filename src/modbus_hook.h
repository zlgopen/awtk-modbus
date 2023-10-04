/**
 * File:   modbus_hook.h
 * Author: AWTK Develop Team
 * Brief:  modbus hook
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

#ifndef TK_MODBUS_HOOK_H
#define TK_MODBUS_HOOK_H

#include "modbus_common.h"

BEGIN_C_DECLS

struct _modbus_hook_t;
typedef struct _modbus_hook_t modbus_hook_t;

typedef ret_t (*modbus_hook_before_request_t)(modbus_hook_t* hook, modbus_req_data_t* req);
typedef ret_t (*modbus_hook_after_request_t)(modbus_hook_t* hook, modbus_resp_data_t* resp,
                                             ret_t code);
typedef ret_t (*modbus_hook_destroy_t)(modbus_hook_t* hook);

/**
 * @class modbus_hook_t
 * 
 * modbus hook
 */
struct _modbus_hook_t {
  modbus_hook_before_request_t before_request;
  modbus_hook_after_request_t after_request;
  modbus_hook_destroy_t destroy;
  void* data;
};

/**
 * @method modbus_hook_before_request
 * 处理modbus请求之前的hook。
 * @param {modbus_hook_t*} hook modbus hook对象。
 * @param {modbus_req_data_t*} req modbus请求数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_hook_before_request(modbus_hook_t* hook, modbus_req_data_t* req);

/**
 * @method modbus_hook_after_request
 * 处理modbus请求之后的hook。
 * @param {modbus_hook_t*} hook modbus hook对象。
 * @param {modbus_resp_data_t*} resp modbus响应数据。
 * @param {ret_t} code modbus响应码。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_hook_after_request(modbus_hook_t* hook, modbus_resp_data_t* resp, ret_t code);

/**
 * @method modbus_hook_destroy
 * 销毁modbus hook。
 * @param {modbus_hook_t*} hook modbus hook对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_hook_destroy(modbus_hook_t* hook);

END_C_DECLS

#endif /*TK_MODBUS_HOOK_H*/

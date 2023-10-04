/**
 * File:   modbus_hook.c
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

#include "modbus_hook.h"

ret_t modbus_hook_before_request(modbus_hook_t* hook, modbus_req_data_t* req) {
  if (hook != NULL && hook->before_request != NULL) {
    return hook->before_request(hook, req);
  }

  return RET_OK;
}

ret_t modbus_hook_after_request(modbus_hook_t* hook, modbus_resp_data_t* resp, ret_t code) {
  if (hook != NULL && hook->after_request != NULL) {
    return hook->after_request(hook, resp, code);
  }

  return RET_OK;
}

ret_t modbus_hook_destroy(modbus_hook_t* hook) {
  if (hook != NULL && hook->destroy != NULL) {
    hook->destroy(hook);
  }
  return RET_OK;
}

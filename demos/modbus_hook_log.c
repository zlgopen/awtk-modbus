/**
 * File:   modbus_hook_log.c
 * Author: AWTK Develop Team
 * Brief:  modbus hook log
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

#include "modbus_hook.h"

static ret_t modbus_hook_log_before_request(modbus_hook_t* hook, modbus_req_data_t* req) {
  log_debug("modbus_hook_log_before_request: %d\n", req->func_code);

  return RET_OK;
}

static ret_t modbus_hook_log_after_request(modbus_hook_t* hook, modbus_resp_data_t* resp,
                                           ret_t code) {
  log_debug("modbus_hook_log_after_request: %d\n", resp->func_code);

  return RET_OK;
}

static ret_t modbus_hook_log_destroy(modbus_hook_t* hook) {
  log_debug("modbus_hook_log_destroy\n");
  return RET_OK;
}

static modbus_hook_t s_hook = {
    .before_request = modbus_hook_log_before_request,
    .after_request = modbus_hook_log_after_request,
    .destroy = modbus_hook_log_destroy,
};

modbus_hook_t* modbus_hook_log_get(void) {
  return &s_hook;
}

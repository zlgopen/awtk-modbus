/**
 * File:   modbus_service_rtu.c
 * Author: AWTK Develop Team
 * Brief:  modbus service rtu
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
 * 2023-10-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "streams/stream_factory.h"
#include "modbus_service_rtu.h"

static event_source_t* s_service_source = NULL;

static ret_t on_service_source_destroy(void* ctx, event_t* e) {
  (void)ctx;
  (void)e;
  s_service_source = NULL;

  return RET_OK;
}

static ret_t modbus_service_rtu_start_impl(event_source_manager_t* esm, const char* url,
                                           modbus_service_args_t* args) {
  event_source_t* source = NULL;
  return_value_if_fail(args != NULL && url != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tk_str_start_with(url, STR_SCHEMA_SERIAL), RET_BAD_PARAMS);
  return_value_if_fail(s_service_source == NULL, RET_FAIL);

  args->is_shared_transport = TRUE;
  return_value_if_fail(tk_service_start_ex(esm, url, modbus_service_create, args, &source) == RET_OK, RET_FAIL);
  return_value_if_fail(source != NULL, RET_FAIL);

  s_service_source = source;
  emitter_on(EMITTER(source), EVT_DESTROY, on_service_source_destroy, NULL);

  return RET_OK;
}

ret_t modbus_service_rtu_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               const char* url, uint8_t slave) {
  static modbus_service_args_t args;
  return_value_if_fail(memory != NULL && url != NULL, RET_BAD_PARAMS);

  args.memory = memory;
  args.proto = MODBUS_PROTO_RTU;
  args.slave = slave;
  return modbus_service_rtu_start_impl(esm, url, &args);
}

ret_t modbus_service_rtu_start_by_args(event_source_manager_t* esm, modbus_service_args_t* args,
                                       const char* url) {
  return modbus_service_rtu_start_impl(esm, url, args);
}

ret_t modbus_service_rtu_stop(void) {
  if (s_service_source != NULL) {
    event_source_manager_t* manager = s_service_source->manager;
    return_value_if_fail(manager != NULL, RET_FAIL);
    return event_source_manager_remove(manager, s_service_source);
  }

  return RET_OK;
}

bool_t modbus_service_rtu_is_started(void) {
  return s_service_source != NULL;
}


/**
 * File:   modbus_service_rtu.c
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

#include "streams/stream_factory.h"
#include "modbus_service_rtu.h"

ret_t modbus_service_rtu_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               modbus_hook_t* hook, const char* url, uint8_t slave) {
  tk_iostream_t* io = NULL;
  modbus_service_t* service = NULL;
  return_value_if_fail(url != NULL, RET_BAD_PARAMS);
  return_value_if_fail(memory != NULL, RET_BAD_PARAMS);
  io = tk_stream_factory_create_iostream(url);
  return_value_if_fail(io != NULL, RET_OOM);

  service = modbus_service_create_with_io(io, MODBUS_PROTO_RTU, memory);
  modbus_service_set_slave(service, slave);
  
  goto_error_if_fail(service != NULL);
  modbus_service_set_hook(service, hook);
  if (esm != NULL) {
    return modbus_service_attach_to_event_source_manager(service, esm);
  } else {
    modbus_service_run(service);
    modbus_service_destroy(service);
    return RET_OK;
  }
error:
  TK_OBJECT_UNREF(io);

  return RET_OOM;
}

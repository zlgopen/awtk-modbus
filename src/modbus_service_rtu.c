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

ret_t modbus_service_rtu_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               const char* url, uint8_t slave) {
  static volatile int i = 0;
  static modbus_service_args_t args;
  return_value_if_fail(memory != NULL && url != NULL, RET_BAD_PARAMS);
  i++;
  assert(i <= 1);
  return_value_if_fail(i <= 1, RET_FAIL);

  args.memory = memory;
  args.proto = MODBUS_PROTO_RTU;
  args.slave = slave;

  return tk_service_start(esm, url, modbus_service_create, &args);
}

ret_t modbus_service_rtu_start_by_args(event_source_manager_t* esm, modbus_service_args_t* args,
                               const char* url) {
  return_value_if_fail(args != NULL && url != NULL, RET_BAD_PARAMS);
  return tk_service_start(esm, url, modbus_service_create, args);
}


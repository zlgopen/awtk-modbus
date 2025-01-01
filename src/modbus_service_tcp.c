/**
 * File:   modbus_service_tcp.c
 * Author: AWTK Develop Team
 * Brief:  modbus service tcp
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

#include "modbus_service_tcp.h"
#ifdef WITH_SOCKET

#include "streams/inet/iostream_tcp.h"

ret_t modbus_service_tcp_start(event_source_manager_t* esm, modbus_memory_t* memory, int port,
                               modbus_proto_t proto, uint8_t slave) {
  char url[128];
  static modbus_service_args_t args;
  return_value_if_fail(memory != NULL, RET_BAD_PARAMS);

  args.memory = memory;
  args.proto = proto;
  args.slave = slave;
  tk_snprintf(url, sizeof(url), "tcp://localhost:%d", port);

  return tk_service_start(esm, url, modbus_service_create, &args);
}
#else
ret_t modbus_service_tcp_start(event_source_manager_t* esm, modbus_memory_t* memory, int port,
                               modbus_proto_t proto, uint8_t slave) {
  return RET_NOT_IMPL;
}
#endif /*WITH_SOCKET*/

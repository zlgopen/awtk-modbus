/**
 * File:   server_ex.c
 * Author: AWTK Develop Team
 * Brief:  modbus server
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
 * 2023-10-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc.h"
#include "modbus_service.h"
#include "modbus_service_rtu.h"
#include "modbus_service_tcp.h"
#include "modbus_memory_default.h"
#include "modbus_memory_custom.h"
#include "tkc/socket_helper.h"
#include "streams/stream_factory.h"
#include "streams/inet/iostream_tcp.h"

#include "server_conf.c"

static ret_t start_rtu(event_source_manager_t* esm, const char* url, modbus_memory_t* memory, uint8_t unit_id) {
  return modbus_service_rtu_start(esm, memory, url, unit_id);
}

static ret_t start_tcp(event_source_manager_t* esm, const char* url, modbus_memory_t* memory, modbus_proto_t proto) {
  const char* p = strrchr(url, ':');
  int port = p != NULL ? tk_atoi(p + 1) : 502;

  return modbus_service_tcp_start(esm, memory, port, proto, MODBUS_DEMO_SLAVE_ID);
}

static ret_t modbus_memory_auto_inc_input_registers(modbus_memory_t* memory) {
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL, RET_BAD_PARAMS);

  if(!s_auto_inc_input_registers) {
    return RET_OK;
  }

  if (memory_default->input_registers_count > 0) {
    uint32_t i = 0;
    for (i = 0; i < memory_default->input_registers_count; i++) {
      memory_default->input_registers_data[i]++;
    }
  }

  return RET_OK;
}

int main(int argc, char* argv[]) {
  modbus_memory_t* memory = NULL;
  event_source_manager_t* esm = NULL;
  const char* url = argc > 1 ? argv[1] : "tcp://localhost:502";
  const char* config = argc > 2 ? argv[2] : "config/default.ini";
  uint8_t unit_id = argc > 3 ? atoi(argv[3]) : 1;

  if (argc == 1) {
    log_debug("usage: %s [url] [config] [unit_id]\n", argv[0]);
    log_debug("ex: %s tcp://localhost:502 config/default.ini\n", argv[0]);
    log_debug("ex: %s serial:///dev/ttys017 config/default.ini 1\n", argv[0]);
    log_debug("ex: %s serial:///dev/ttys017 config/default.ini 2\n", argv[0]);
    return 0;
  }

  memory = server_conf_load(config);
  return_value_if_fail(memory != NULL, -1);

  platform_prepare();
  tk_socket_init();

  esm = event_source_manager_default_create();

  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    start_tcp(esm, url, memory, MODBUS_PROTO_TCP);
  } else if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP)) {
    start_tcp(esm, url, memory, MODBUS_PROTO_RTU);
  } else {
    start_rtu(esm, url, memory, unit_id);
  }

  while(1) {
    event_source_manager_dispatch(esm);
    modbus_memory_auto_inc_input_registers(memory);
    sleep_ms(50);
  }

  event_source_manager_destroy(esm);

  modbus_memory_destroy(memory);
  tk_socket_deinit();

  return 0;
}

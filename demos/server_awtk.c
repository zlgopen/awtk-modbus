/**
 * File:   server_awtk.c
 * Author: AWTK Develop Team
 * Brief:  modbus server with awtk.
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

#include "awtk.h"
#include "modbus_service_tcp.h"
#include "modbus_service_rtu.h"
#include "modbus_hook_myapp.c"

#define ON_CMD_LINE modbus_on_cmd_line
static const char* s_url = "tcp://localhost:502";

static ret_t modbus_on_cmd_line(int argc, char* argv[]) {
  if (argc > 1) {
    s_url = argv[1];
  }

  return RET_OK;
}

static ret_t on_close_clicked(void* ctx, event_t* e) {
  tk_quit();
  return RET_OK;
}

static ret_t main_window_create(void) {
  widget_t* win = window_open("main");

  widget_child_on(win, "close", EVT_CLICK, on_close_clicked, NULL);

  return RET_OK;
}

static modbus_memory_t* s_memory = NULL;
ret_t application_init(void) {
  const char* url = s_url;
  event_source_manager_t* esm = main_loop_get_event_source_manager(main_loop());
  modbus_memory_t* memory = modbus_memory_create(
      MODBUS_DEMO_BITS_ADDRESS, MODBUS_DEMO_BITS_NB, MODBUS_DEMO_INPUT_BITS_ADDRESS,
      MODBUS_DEMO_INPUT_BITS_NB, MODBUS_DEMO_REGISTERS_ADDRESS, MODBUS_DEMO_REGISTERS_NB,
      MODBUS_DEMO_INPUT_REGISTERS_ADDRESS, MODBUS_DEMO_INPUT_REGISTERS_NB);
  modbus_hook_t* hook = modbus_hook_myapp_get(memory);
  socket_init();
  main_window_create();
  s_memory = memory;

  if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) || tk_str_start_with(url, STR_SCHEMA_TCP)) {
    const char* p = strrchr(url, ':');
    int port = p != NULL ? tk_atoi(p + 1) : 502;
    modbus_proto_t proto =
        tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
    return modbus_service_tcp_start(esm, memory, hook, port, proto, MODBUS_DEMO_SLAVE_ID);
  } else {
    return modbus_service_rtu_start(esm, memory, hook, url, MODBUS_DEMO_SLAVE_ID);
  }
}

ret_t application_exit(void) {
  socket_deinit();
  modbus_memory_destroy(s_memory);

  return RET_OK;
}

#include "../res/assets.inc"
#include "awtk_main.inc"

/**
 * File:   server.c
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

#include "modbus_service.h"
#include "modbus_service_rtu.h"
#include "modbus_service_tcp.h"
#include "modbus_memory.h"
#include "modbus_hook_myapp.c"
#include "tkc/socket_helper.h"
#include "streams/stream_factory.h"
#include "streams/inet/iostream_tcp.h"

static ret_t start_rtu(const char* url, modbus_memory_t* memory) {
  modbus_hook_t* hook = modbus_hook_myapp_get(memory);

  return modbus_service_rtu_start(NULL, memory, hook, url);
}

static ret_t start_tcp(const char* url, modbus_memory_t* memory, modbus_proto_t proto) {
  const char* p = strrchr(url, ':');
  int port = p != NULL ? tk_atoi(p + 1) : 502;
  modbus_hook_t* hook = modbus_hook_myapp_get(memory);

  return modbus_service_tcp_start(NULL, memory, hook, port, proto);
}

/*生成测试数据*/
static void modbus_memory_init_demo_data(modbus_memory_t* memory) {
  uint32_t i = 0;

  for (i = 0; i < memory->input_bits_count; i++) {
    memory->input_bits_data[i] = i % 2;
  }

  for (i = 0; i < memory->input_registers_count; i++) {
    memory->input_registers_data[i] = (uint16_t)i;
  }

  return;
}

int main(int argc, char* argv[]) {
  modbus_memory_t* memory = NULL;
  const char* url = argc > 1 ? argv[1] : "serial:///dev/ttys124";

  platform_prepare();
  tk_socket_init();

  memory = modbus_memory_create(
      MODBUS_DEMO_BITS_ADDRESS, MODBUS_DEMO_BITS_NB, MODBUS_DEMO_INPUT_BITS_ADDRESS,
      MODBUS_DEMO_INPUT_BITS_NB, MODBUS_DEMO_REGISTERS_ADDRESS, MODBUS_DEMO_REGISTERS_NB,
      MODBUS_DEMO_INPUT_REGISTERS_ADDRESS, MODBUS_DEMO_INPUT_REGISTERS_NB);

  modbus_memory_init_demo_data(memory);

  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    start_tcp(url, memory, MODBUS_PROTO_TCP);
  } else if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP)) {
    start_tcp(url, memory, MODBUS_PROTO_RTU);
  } else {
    start_rtu(url, memory);
  }

  modbus_memory_destroy(memory);
  tk_socket_deinit();

  return 0;
}

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

#include "tkc.h"
#include "modbus_service.h"
#include "modbus_service_rtu.h"
#include "modbus_service_tcp.h"
#include "modbus_memory_default.h"
#include "modbus_memory_custom.h"
#include "tkc/socket_helper.h"
#include "streams/stream_factory.h"
#include "streams/inet/iostream_tcp.h"

static ret_t start_rtu(event_source_manager_t* esm, const char* url, modbus_memory_t* memory) {
  return modbus_service_rtu_start(esm, memory, url, MODBUS_DEMO_SLAVE_ID);
}

static ret_t start_tcp(event_source_manager_t* esm, const char* url, modbus_memory_t* memory, modbus_proto_t proto) {
  const char* p = strrchr(url, ':');
  int port = p != NULL ? tk_atoi(p + 1) : 502;

  return modbus_service_tcp_start(esm, memory, port, proto, MODBUS_DEMO_SLAVE_ID);
}

/*生成测试数据*/
static void modbus_memory_init_demo_data(modbus_memory_t* memory) {
  uint32_t i = 0;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);

  if (memory_default == NULL) {
    return;
  }

  for (i = 0; i < memory_default->input_bits_count; i++) {
    memory_default->input_bits_data[i] = i % 2;
  }

  for (i = 0; i < memory_default->input_registers_count; i++) {
    memory_default->input_registers_data[i] = (uint16_t)i;
  }

  return;
}

int main(int argc, char* argv[]) {
  modbus_memory_t* memory = NULL;
  const char* url = argc > 1 ? argv[1] : "tcp://localhost:502";
  bool_t use_custom = argc < 3 ? TRUE : FALSE;
  event_source_manager_t* esm = NULL;

  platform_prepare();
  tk_socket_init();

  if (use_custom) {
    log_debug("use custom memory.\n");
    memory = modbus_memory_custom_create();
  } else {
    log_debug("use default memory.\n");
    memory = modbus_memory_default_create(
        MODBUS_DEMO_BITS_ADDRESS, MODBUS_DEMO_BITS_NB, MODBUS_DEMO_INPUT_BITS_ADDRESS,
        MODBUS_DEMO_INPUT_BITS_NB, MODBUS_DEMO_REGISTERS_ADDRESS, MODBUS_DEMO_REGISTERS_NB,
        MODBUS_DEMO_INPUT_REGISTERS_ADDRESS, MODBUS_DEMO_INPUT_REGISTERS_NB);
  }

  modbus_memory_init_demo_data(memory);
  esm = event_source_manager_default_create();

  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    start_tcp(esm, url, memory, MODBUS_PROTO_TCP);
  } else if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP)) {
    start_tcp(esm, url, memory, MODBUS_PROTO_RTU);
  } else {
    start_rtu(esm, url, memory);
  }

  while(1) {
    event_source_manager_dispatch(esm);

    sleep_ms(50);
  }

  event_source_manager_destroy(esm);

  modbus_memory_destroy(memory);
  tk_socket_deinit();

  return 0;
}

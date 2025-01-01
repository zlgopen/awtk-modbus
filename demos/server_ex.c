/**
 * File:   server_ex.c
 * Author: AWTK Develop Team
 * Brief:  modbus server
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
#include "conf_io/conf_json.h"

#include "server_conf.c"

static ret_t start_rtu(event_source_manager_t* esm, const char* url, modbus_memory_t* memory,
                       uint8_t unit_id) {
  return modbus_service_rtu_start(esm, memory, url, unit_id);
}

static ret_t start_tcp(event_source_manager_t* esm, const char* url, modbus_memory_t* memory,
                       modbus_proto_t proto) {
  const char* p = strrchr(url, ':');
  int port = p != NULL ? tk_atoi(p + 1) : 502;

  return modbus_service_tcp_start(esm, memory, port, proto, MODBUS_DEMO_SLAVE_ID);
}

static ret_t update_input_registers(modbus_memory_default_t* memory_default) {
  if (s_auto_inc_input_registers) {
    uint16_t* data = (uint16_t*)(memory_default->input_registers->data);
    uint32_t i = 0;
    for (i = 0; i < memory_default->input_registers->length; i++) {
      data[i]++;
    }

    log_debug("index=%d\n", (int)(*data));
  }

  return RET_OK;
}

static ret_t start_modbus_server_with_conf(const char* filename) {
  uint32_t size = 0;
  const char* url = NULL;
  uint8_t unit_id = 0xff;
  conf_doc_t* doc = NULL;
  modbus_memory_t* memory = NULL;
  event_source_manager_t* esm = NULL;
  modbus_memory_default_t* memory_default = NULL;
  char* data = (char*)file_read(filename, &size);
  return_value_if_fail(data != NULL, RET_OOM);

  doc = conf_doc_load_json(data, size);
  TKMEM_FREE(data);
  goto_error_if_fail(doc != NULL);
  memory = server_conf_load_doc(doc);
  url = conf_doc_get_str(doc, "url", NULL);

  if (tk_str_start_with(url, "serial://")) {
    unit_id = conf_doc_get_int(doc, "unit_id", 1);
  } else {
    unit_id = conf_doc_get_int(doc, "unit_id", 0xff);
  }
  log_debug("url=%s unit_id=%d\n", url, unit_id);
  esm = event_source_manager_default_create();

  goto_error_if_fail(memory != NULL && url != NULL && esm != NULL);
  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    start_tcp(esm, url, memory, MODBUS_PROTO_TCP);
  } else if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP)) {
    start_tcp(esm, url, memory, MODBUS_PROTO_RTU);
  } else {
    start_rtu(esm, url, memory, unit_id);
  }
  conf_doc_destroy(doc);
  doc = NULL;

  memory_default = MODBUS_MEMORY_DEFAULT(memory);
  while (1) {
    event_source_manager_dispatch(esm);
    sleep_ms(100);
    update_input_registers(memory_default);
  }

  event_source_manager_destroy(esm);

error:
  if (doc != NULL) {
    conf_doc_destroy(doc);
  }

  if (memory != NULL) {
    modbus_memory_destroy(memory);
  }

  return RET_OK;
}

int main(int argc, char* argv[]) {
  const char* config = argc > 1 ? argv[1] : "config/default.json";

  if (argc == 1) {
    log_debug("usage: %s [config]\n", argv[0]);
    log_debug("ex: %s config/default.json\n", argv[0]);
    return 0;
  }

  platform_prepare();
  tk_socket_init();
  start_modbus_server_with_conf(config);
  tk_socket_deinit();

  return 0;
}

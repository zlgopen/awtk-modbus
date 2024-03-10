/**
 * File:   server_conf.c
 * Author: AWTK Develop Team
 * Brief:  server_conf
 *
 * Copyright (c) 2023 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-02-11 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "modbus_client.h"
#include "tkc/socket_helper.h"
#include "conf_io/conf_node.h"
#include "conf_io/conf_ini.h"
#include "modbus_memory_default.h"

static bool_t s_auto_inc_input_registers = FALSE;

static modbus_memory_t* modbus_memory_init_data(modbus_memory_t* m, conf_doc_t* doc) {
  uint32_t i = 0;
  tokenizer_t t;
  const char* init = NULL;
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(m);
  return_value_if_fail(memory_default != NULL, NULL);

  init = conf_doc_get_str(doc, "bits.init", NULL);
  if (init != NULL) {
    i = 0;
    log_debug("bits.init:\t\t%s\n", init);
    tokenizer_init(&t, init, tk_strlen(init), ", ");
    while (tokenizer_has_more(&t) && i < memory_default->bits->length) {
      int32_t v = tokenizer_next_int(&t, 0);
      memory_default->bits->data[i] = v != 0;
      i++;
    }
    tokenizer_deinit(&t);
  }

  init = conf_doc_get_str(doc, "input_bits.init", NULL);
  if (init != NULL) {
    i = 0;
    log_debug("input_bits.init:\t%s\n", init);
    tokenizer_init(&t, init, tk_strlen(init), ", ");
    while (tokenizer_has_more(&t) && i < memory_default->input_bits->length) {
      int32_t v = tokenizer_next_int(&t, 0);
      memory_default->input_bits->data[i] = v != 0;
      i++;
    }
    tokenizer_deinit(&t);
  }

  init = conf_doc_get_str(doc, "registers.init", NULL);
  if (init != NULL) {
    i = 0;
    log_debug("registers.init:\t\t%s\n", init);
    tokenizer_init(&t, init, tk_strlen(init), ", ");
    while (tokenizer_has_more(&t) && i < memory_default->registers->length) {
      int32_t v = tokenizer_next_int(&t, 0);
      memory_default->registers->data[i] = v;
      i++;
    }
    tokenizer_deinit(&t);
  }

  init = conf_doc_get_str(doc, "input_registers.init", NULL);
  if (init != NULL) {
    i = 0;
    log_debug("input_registers.init:\t%s\n", init);
    tokenizer_init(&t, init, tk_strlen(init), ", ");
    while (tokenizer_has_more(&t) && i < memory_default->input_registers->length) {
      int32_t v = tokenizer_next_int(&t, 0);
      memory_default->input_registers->data[i] = v;
      i++;
    }
    tokenizer_deinit(&t);
  }

  return m;
}

static modbus_memory_t* server_conf_load_doc(conf_doc_t* doc) {
  modbus_memory_t* m = NULL;
  conf_node_t* channels = conf_node_find_child(doc->root, "channels");
  s_auto_inc_input_registers = conf_doc_get_bool(doc, "input_registers.auto_inc", FALSE);

  m = modbus_memory_default_create_with_conf(channels);
  return_value_if_fail(m != NULL, NULL);

  return modbus_memory_init_data(m, doc);
}

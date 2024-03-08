/**
 * File:   client.c
 * Author: AWTK Develop Team
 * Brief:  modbus client
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

#include "modbus_client.h"
#include "tkc/socket_helper.h"
#include "conf_io/conf_node.h"
#include "conf_io/conf_ini.h"

static ret_t conf_get_params(conf_node_t* iter, uint16_t* addr, uint16_t* count,
                             const char** data) {
  value_t v;
  if (conf_node_get_child_value(iter, "addr", &v) != RET_OK) {
    log_debug("get addr failed\n");
    return RET_FAIL;
  }
  *addr = value_uint16(&v);

  if (conf_node_get_child_value(iter, "count", &v) != RET_OK) {
    *count = 1;
  } else {
    *count = value_uint16(&v);
  }

  if (conf_node_get_child_value(iter, "data", &v) == RET_OK) {
    *data = value_str(&v);
  }

  return RET_OK;
}

static ret_t run_read_bits(const char* name, modbus_client_t* client, conf_node_t* iter) {
  ret_t ret = RET_OK;
  uint16_t addr = 0;
  uint16_t count = 0;
  const char* data = NULL;
  uint8_t bits[1024] = {0};

  if (conf_get_params(iter, &addr, &count, &data) != RET_OK) {
    return RET_OK;
  }

  if (tk_str_eq(name, "read_bits")) {
    ret = modbus_client_read_bits(client, addr, count, bits);
  } else {
    ret = modbus_client_read_input_bits(client, addr, count, bits);
  }

  if (ret == RET_OK) {
    if (data != NULL) {
      uint32_t i = 0;
      tokenizer_t t;
      tokenizer_init(&t, data, tk_strlen(data), ", ");

      while (tokenizer_has_more(&t)) {
        uint8_t data_iter = (uint8_t)tokenizer_next_int(&t, 0);
        if (data_iter != bits[i]) {
          log_debug("bit %d data:%d, actual:%d\n", i, data_iter, bits[i]);
        }
        i++;
      }

      tokenizer_deinit(&t);
    } else {
      log_debug("----------read result----------\n");
      for (uint32_t i = 0; i < count; i++) {
        log_debug("bit %d:%d\n", i, bits[i]);
      }
      log_debug("-------------------------------\n");
    }

    log_debug("%s ok\n", name);
  } else {
    log_debug("%s failed:%s\n", name, modbus_common_get_last_exception_str(&client->common));
  }

  return ret;
}

static ret_t run_read_registers(const char* name, modbus_client_t* client, conf_node_t* iter) {
  ret_t ret = RET_OK;
  uint16_t addr = 0;
  uint16_t count = 0;
  const char* data = NULL;
  uint16_t registers[1024] = {0};

  if (conf_get_params(iter, &addr, &count, &data) != RET_OK) {
    return RET_OK;
  }

  if (tk_str_eq(name, "read_registers")) {
    ret = modbus_client_read_registers(client, addr, count, registers);
  } else {
    ret = modbus_client_read_input_registers(client, addr, count, registers);
  }

  if (ret == RET_OK) {
    if (data != NULL) {
      uint32_t i = 0;
      tokenizer_t t;
      tokenizer_init(&t, data, tk_strlen(data), ", ");

      while (tokenizer_has_more(&t)) {
        uint16_t data_iter = (uint16_t)tokenizer_next_int(&t, 0);
        if (data_iter != registers[i]) {
          log_debug("register %d data:%d, actual:%d\n", i, data_iter, registers[i]);
        }
        i++;
      }

      tokenizer_deinit(&t);
    } else {
      log_debug("----------read result----------\n");
      for (uint32_t i = 0; i < count; i++) {
        log_debug("register %d:%d\n", i, registers[i]);
      }
      log_debug("-------------------------------\n");
    }

    log_debug("%s ok\n", name);
  } else {
    log_debug("%s failed:%s\n", name, modbus_common_get_last_exception_str(&client->common));
  }

  return ret;
}

static ret_t run_write_bits(const char* name, modbus_client_t* client, conf_node_t* iter) {
  ret_t ret = RET_OK;
  uint16_t addr = 0;
  uint16_t count = 0;
  const char* data = NULL;
  uint8_t bits[1024] = {0};
  tokenizer_t t;

  if (conf_get_params(iter, &addr, &count, &data) != RET_OK) {
    return RET_OK;
  }

  if (data != NULL) {
    uint32_t i = 0;
    tokenizer_init(&t, data, tk_strlen(data), ", ");
    while (tokenizer_has_more(&t)) {
      uint8_t data_iter = (uint8_t)tokenizer_next_int(&t, 0);
      bits[i] = data_iter;
      i++;
    }
    tokenizer_deinit(&t);
    if (count > i) {
      count = i;
    }
  }

  if (tk_str_eq(name, "write_bit")) {
    ret = modbus_client_write_bit(client, addr, bits[0]);
  } else {
    ret = modbus_client_write_bits(client, addr, count, bits);
  }

  if (ret == RET_OK) {
    log_debug("%s ok\n", name);
  } else {
    log_debug("%s failed:%s\n", name, modbus_common_get_last_exception_str(&client->common));
  }

  return ret;
}

#include "tkc/date_time.h"
static uint32_t s_count = 0;
static uint16_t parse_register_value(const char* value) {
  date_time_t dt;
  date_time_init(&dt);
  uint16_t ret = 0;

  if (tk_str_eq(value, "second")) {
    ret = dt.second;
  } else if (tk_str_eq(value, "minute")) {
    ret = dt.minute;
  } else if (tk_str_eq(value, "hour")) {
    ret = dt.hour;
  } else if (tk_str_eq(value, "day")) {
    ret = dt.day;
  } else if (tk_str_eq(value, "month")) {
    ret = dt.month;
  } else if (tk_str_eq(value, "year")) {
    ret = dt.year;
  } else if (tk_str_eq(value, "rand")) {
    ret = random();
  } else if (tk_str_eq(value, "count")) {
    ret = s_count++;
  } else {
    return tk_atoi(value);
  }

  log_debug("%s => %d\n", value, ret);
  return ret;
}

static ret_t run_write_registers(const char* name, modbus_client_t* client, conf_node_t* iter) {
  ret_t ret = RET_OK;
  uint16_t addr = 0;
  uint16_t count = 0;
  const char* data = NULL;
  uint16_t registers[1024] = {0};
  tokenizer_t t;

  if (conf_get_params(iter, &addr, &count, &data) != RET_OK) {
    return RET_OK;
  }

  if (data != NULL) {
    uint32_t i = 0;
    tokenizer_init(&t, data, tk_strlen(data), ", ");
    while (tokenizer_has_more(&t)) {
      const char* value = tokenizer_next_str(&t);
      registers[i] = parse_register_value(value);
      i++;
    }
    tokenizer_deinit(&t);
    if (count > i) {
      count = i;
    }
  }

  if (tk_str_eq(name, "write_register")) {
    ret = modbus_client_write_register(client, addr, registers[0]);
  } else {
    ret = modbus_client_write_registers(client, addr, count, registers);
  }

  if (ret == RET_OK) {
    log_debug("%s ok\n", name);
  } else {
    log_debug("%s failed:%s\n", name, modbus_common_get_last_exception_str(&client->common));
  }

  return ret;
}

static void demo_tcp(void) {
  uint8_t read_bits_result[4];
  uint8_t write_bits[] = {TRUE, FALSE, TRUE, FALSE};
  uint16_t read_registers_result[4];
  uint16_t write_registers[] = {0x1122, 0x2233, 0x3344, 0x4455};

  modbus_client_t* client = modbus_client_create("tcp://localhost:502");
  /*
   * RTU: Windows style
   * modbus_client_t* client = modbus_client_create("serial://COM5");
   */
  /*
   * RTU: Linux style
   * modbus_client_t* client = modbus_client_create("serial:///dev/ttys125");
   */

  ENSURE(modbus_client_write_bit(client, MODBUS_DEMO_BITS_ADDRESS, 1) == RET_OK);
  ENSURE(modbus_client_read_bits(client, MODBUS_DEMO_BITS_ADDRESS, 1, read_bits_result) == RET_OK);
  ENSURE(read_bits_result[0] == TRUE);

  ENSURE(modbus_client_write_bits(client, MODBUS_DEMO_BITS_ADDRESS, 4, write_bits) == RET_OK);
  ENSURE(modbus_client_read_bits(client, MODBUS_DEMO_BITS_ADDRESS, 4, read_bits_result) == RET_OK);
  ENSURE(read_bits_result[0] == TRUE);
  ENSURE(read_bits_result[1] == FALSE);
  ENSURE(read_bits_result[2] == TRUE);
  ENSURE(read_bits_result[3] == FALSE);

  ENSURE(modbus_client_write_register(client, MODBUS_DEMO_REGISTERS_ADDRESS, 0xaabb) == RET_OK);
  ENSURE(modbus_client_read_registers(client, MODBUS_DEMO_REGISTERS_ADDRESS, 1,
                                      read_registers_result) == RET_OK);
  ENSURE(read_registers_result[0] == 0xaabb);

  ENSURE(modbus_client_write_registers(client, MODBUS_DEMO_REGISTERS_ADDRESS, 4, write_registers) ==
         RET_OK);
  ENSURE(modbus_client_read_registers(client, MODBUS_DEMO_REGISTERS_ADDRESS, 4,
                                      read_registers_result) == RET_OK);
  ENSURE(read_registers_result[0] == 0x1122);
  ENSURE(read_registers_result[1] == 0x2233);
  ENSURE(read_registers_result[2] == 0x3344);
  ENSURE(read_registers_result[3] == 0x4455);

  ENSURE(modbus_client_read_input_bits(client, MODBUS_DEMO_INPUT_BITS_ADDRESS, 1,
                                       read_bits_result) == RET_OK);
  ENSURE(modbus_client_read_input_registers(client, MODBUS_DEMO_INPUT_REGISTERS_ADDRESS, 1,
                                            read_registers_result) == RET_OK);

  modbus_client_destroy(client);
}

static void run_script(conf_doc_t* doc, uint32_t times) {
  modbus_client_t* client = NULL;
  conf_node_t* iter = conf_node_get_first_child(doc->root);

  while (iter != NULL) {
    value_t v;
    const char* name = conf_node_get_name(iter);

    if (tk_str_eq(name, "connect")) {
      uint8_t slave = 17;
      const char* url = NULL;

      if (conf_node_get_child_value(iter, "url", &v) != RET_OK) {
        log_debug("get url failed");
        break;
      }

      if (client != NULL) {
        iter = iter->next;
        continue;
      }

      url = value_str(&v);
      client = modbus_client_create(url);
      if (conf_node_get_child_value(iter, "slave", &v) == RET_OK) {
        slave = value_uint8(&v);
        modbus_client_set_slave(client, slave);
      }

      log_debug("connect: %s\n", url);
      iter = iter->next;
      continue;
    }

    if (client == NULL) {
      log_debug("client is null.\n");
      break;
    }

    if (tk_str_eq(name, "read_bits") || tk_str_eq(name, "read_input_bits")) {
      run_read_bits(name, client, iter);
    } else if (tk_str_eq(name, "read_registers") || tk_str_eq(name, "read_input_registers")) {
      run_read_registers(name, client, iter);
    } else if (tk_str_eq(name, "write_bit") || tk_str_eq(name, "write_bits")) {
      run_write_bits(name, client, iter);
    } else if (tk_str_eq(name, "write_register") || tk_str_eq(name, "write_registers")) {
      run_write_registers(name, client, iter);
    } else if (tk_str_eq(name, "sleep")) {
      uint32_t ms = conf_node_get_child_value_int32(iter, "time", 0);
      log_debug("sleep %d\n", ms);
      sleep_ms(ms);
    } else if (tk_str_eq(name, "rewind")) {
      iter = conf_node_get_first_child(doc->root);
      log_debug("rewind\n");
      continue;
    } else if (tk_str_eq(name, "goto")) {
      const char* target = conf_node_get_child_value_str(iter, "target", NULL);
      iter = conf_node_get_first_child(doc->root);
      while (iter != NULL) {
        if (tk_str_eq(conf_node_get_name(iter), target)) {
          log_debug("goto %s\n", target);
          break;
        }
        iter = iter->next;
      }
      continue;
    } else if (tk_str_eq(name, "close")) {
      modbus_client_destroy(client);
      client = NULL;
    } else {
      log_debug("unknown command:%s\n", name);
    }

    iter = iter->next;

    if (times == 0) {
      break;
    } else if (iter == NULL) {
      iter = conf_node_get_first_child(doc->root);
      times--;
      log_debug("=============%u===============\n", times);
      if (times == 0) {
        break;
      }
    }
  }

  if (client != NULL) {
    modbus_client_destroy(client);
  }
}

int main(int argc, char* argv[]) {
  char* data = NULL;
  conf_doc_t* doc = NULL;
  const char* input = argc > 1 ? argv[1] : "data/client_default.ini";
  uint32_t times = argc > 2 ? tk_atoi(argv[2]) : 1;

  platform_prepare();

  if (argc < 2) {
    log_debug("Usage: %s config times\n", argv[0]);
    log_debug(" ex: %s data/tcp.ini\n", argv[0]);
    log_debug(" ex: %s data/tcp.ini 10\n", argv[0]);
    return 0;
  }

  tk_socket_init();

  data = (char*)file_read(input, NULL);
  if (data != NULL) {
    doc = conf_doc_load_ini(data);
    if (doc != NULL) {
      run_script(doc, times);
      conf_doc_destroy(doc);
    }
    TKMEM_FREE(data);
  } else if (tk_str_eq(input, "demo_tcp")) {
    demo_tcp();
  }

  tk_socket_deinit();

  return 0;
}

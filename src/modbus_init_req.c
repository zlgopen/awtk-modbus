/**
 * File:   modbus_init_req.c
 * Author: AWTK Develop Team
 * Brief:  modbus_init_req
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
 * 2024-03-07 Li XianJing <lixianjing@zlg.cn> created.
 *
 */

#include "modbus_init_req.h"

static ret_t modbus_init_req_load(modbus_init_req_t* init, conf_node_t* node) {
  return_value_if_fail(init != NULL && node != NULL, RET_BAD_PARAMS);

  init->access_type = conf_node_get_child_value_int32(node, "access_type", 0);
  init->offset = conf_node_get_child_value_int32(node, "offset", 0);
  init->length = conf_node_get_child_value_int32(node, "length", 0);
  init->value = conf_node_get_child_value_int32(node, "value", 0);
  init->unit_id = conf_node_get_child_value_int32(node, "unit_id", 0);

  log_debug("modbus_init_load unit_id=%d access_type=%d offset=%d length=%d value=%d\n",
            init->unit_id, init->access_type, init->offset, init->length, init->value);

  return RET_OK;
}

modbus_init_req_t* modbus_init_req_create(conf_node_t* node) {
  modbus_init_req_t* init = TKMEM_ZALLOC(modbus_init_req_t);
  return_value_if_fail(init != NULL, NULL);

  if (modbus_init_req_load(init, node) != RET_OK) {
    modbus_init_req_destroy(init);
    return NULL;
  }

  return init;
}

ret_t modbus_init_req_request(modbus_init_req_t* init, modbus_client_t* client) {
  ret_t ret = RET_OK;
  return_value_if_fail(init != NULL && client != NULL, RET_BAD_PARAMS);

  if (init->unit_id) {
    modbus_client_set_slave(client, init->unit_id);
  }

  log_debug("modbus_init_req_request %d %d %d %d\n", init->access_type, init->offset, init->length,
            init->value);

  switch (init->access_type) {
    case MODBUS_FC_WRITE_SINGLE_COIL: {
      ret = modbus_client_write_bit(client, init->offset, init->value);
      break;
    }
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER: {
      ret = modbus_client_write_register(client, init->offset, init->value);
      break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_COILS: {
      uint8_t* values = TKMEM_ALLOC(init->length);
      memset(values, init->value, init->length);
      ret = modbus_client_write_bits(client, init->offset, init->length, values);
      TKMEM_FREE(values);
      break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
      uint16_t* values = TKMEM_ALLOC(init->length * sizeof(uint16_t));
      uint32_t i = 0;
      for (i = 0; i < init->length; i++) {
        values[i] = init->value;
      }
      ret = modbus_client_write_registers(client, init->offset, init->length, values);
      TKMEM_FREE(values);
      break;
    }
    default: {
      return RET_NOT_IMPL;
    }
  }

  return ret;
}

ret_t modbus_init_req_destroy(modbus_init_req_t* init) {
  return_value_if_fail(init != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(init);

  return RET_OK;
}

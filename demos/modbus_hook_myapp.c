/**
 * File:   modbus_hook_myapp.c
 * Author: AWTK Develop Team
 * Brief:  modbus hook log
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

#include "modbus_hook.h"

static ret_t modbus_hook_myapp_before_request(modbus_hook_t* hook, modbus_req_data_t* req) {
  modbus_memory_t* memory = (modbus_memory_t*)(hook->data);

  switch (req->func_code) {
    case MODBUS_FC_READ_HOLDING_REGISTERS: {
      uint16_t* registers = memory->registers_data;
      uint16_t addr = req->addr;
      uint16_t count = req->count;
      uint16_t offset = addr - memory->registers_start;

      /*prepare data*/
      break;
    }
    case MODBUS_FC_READ_INPUT_REGISTERS: {
      uint16_t* registers = memory->input_registers_data;
      /*prepare data*/
      break;
    }
    case MODBUS_FC_READ_COILS: {
      uint8_t* bits = memory->bits_data;
      /*prepare data*/
      break;
    }
    case MODBUS_FC_READ_DISCRETE_INPUTS: {
      uint8_t* bits = memory->input_bits_data;
      /*prepare data*/
      break;
    }
    default:
      break;
  }

  log_debug("modbus_hook_myapp_before_request: %d\n", req->func_code);
  return RET_OK;
}

static ret_t modbus_hook_myapp_after_request(modbus_hook_t* hook, modbus_resp_data_t* resp,
                                             ret_t code) {
  log_debug("modbus_hook_myapp_after_request: %d\n", resp->func_code);
  modbus_memory_t* memory = (modbus_memory_t*)(hook->data);
  switch (resp->func_code) {
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER:
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
      uint16_t* registers = memory->registers_data;
      /*flush data*/
      break;
    }
    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_MULTIPLE_COILS: {
      uint8_t* bits = memory->bits_data;
      /*data*/
      break;
    }
    default:
      break;
  }
  return RET_OK;
}

static ret_t modbus_hook_myapp_destroy(modbus_hook_t* hook) {
  log_debug("modbus_hook_myapp_destroy\n");
  return RET_OK;
}

static modbus_hook_t s_hook = {
    .before_request = modbus_hook_myapp_before_request,
    .after_request = modbus_hook_myapp_after_request,
    .destroy = modbus_hook_myapp_destroy,
};

modbus_hook_t* modbus_hook_myapp_get(modbus_memory_t* memory) {
  s_hook.data = memory;
  return &s_hook;
}

/**
 * File:   modbus_memory_default.c
 * Author: AWTK Develop Team
 * Brief:  modbus memory default
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
 * 2024-03-10 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "modbus_memory_default.h"

static ret_t modbus_memory_default_read_bits(modbus_memory_t* memory, uint16_t addr, uint16_t count,
                                             uint8_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  return modbus_server_channel_read_bits(m->bits, addr, count, buff);
}

static ret_t modbus_memory_default_read_input_bits(modbus_memory_t* memory, uint16_t addr,
                                                   uint16_t count, uint8_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  return modbus_server_channel_read_bits(m->input_bits, addr, count, buff);
}

static ret_t modbus_memory_default_read_registers(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t count, uint16_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  return modbus_server_channel_read_registers(m->registers, addr, count, buff);
}

static ret_t modbus_memory_default_read_input_registers(modbus_memory_t* memory, uint16_t addr,
                                                        uint16_t count, uint16_t* buff) {
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  return modbus_server_channel_read_registers(m->input_registers, addr, count, buff);
}

static ret_t modbus_memory_default_write_bit(modbus_memory_t* memory, uint16_t addr,
                                             uint8_t value) {
  ret_t ret = RET_OK;
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  ret = modbus_server_channel_write_bit(m->bits, addr, value);
  if (ret == RET_OK) {
    emitter_dispatch_simple_event(m->emitter, EVT_PROPS_CHANGED);
  }

  return ret;
}

static ret_t modbus_memory_default_write_bits(modbus_memory_t* memory, uint16_t addr,
                                              uint16_t count, const uint8_t* buff) {
  ret_t ret = RET_OK;                                                
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  ret = modbus_server_channel_write_bits(m->bits, addr, count, buff);
  if (ret == RET_OK) {
    emitter_dispatch_simple_event(m->emitter, EVT_PROPS_CHANGED);
  }

  return ret;
}

static ret_t modbus_memory_default_write_register(modbus_memory_t* memory, uint16_t addr,
                                                  uint16_t value) {
  ret_t ret = RET_OK;                                                    
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  ret = modbus_server_channel_write_register(m->registers, addr, value);
  if (ret == RET_OK) {
    emitter_dispatch_simple_event(m->emitter, EVT_PROPS_CHANGED);
  }

  return ret;
}

static ret_t modbus_memory_default_write_registers(modbus_memory_t* memory, uint16_t addr,
                                                   uint16_t count, const uint16_t* buff) {
  ret_t ret = RET_OK;                                                    
  modbus_memory_default_t* m = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(m != NULL, RET_BAD_PARAMS);

  ret = modbus_server_channel_write_registers(m->registers, addr, count, buff);
  if (ret == RET_OK) {
    emitter_dispatch_simple_event(m->emitter, EVT_PROPS_CHANGED);
  }

  return ret;
}

static ret_t modbus_memory_default_destroy(modbus_memory_t* memory) {
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  return_value_if_fail(memory_default != NULL, RET_BAD_PARAMS);

  modbus_server_channel_destroy(memory_default->bits);
  modbus_server_channel_destroy(memory_default->input_bits);
  modbus_server_channel_destroy(memory_default->registers);
  modbus_server_channel_destroy(memory_default->input_registers);

  emitter_destroy(memory_default->emitter);
  TKMEM_FREE(memory_default);

  return RET_OK;
}

modbus_memory_t* modbus_memory_default_create_test(void) {
  modbus_server_channel_t* bits = modbus_server_channel_create(
      MODBUS_SERVER_CHANNEL_BITS, MODBUS_DEMO_BITS_ADDRESS, MODBUS_DEMO_BITS_NB, TRUE);
  modbus_server_channel_t* input_bits =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_INPUT_BITS, MODBUS_DEMO_INPUT_BITS_ADDRESS,
                                   MODBUS_DEMO_INPUT_BITS_NB, FALSE);
  modbus_server_channel_t* registers =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_REGISTERS, MODBUS_DEMO_REGISTERS_ADDRESS,
                                   MODBUS_DEMO_REGISTERS_NB, TRUE);
  modbus_server_channel_t* input_registers = modbus_server_channel_create(
      MODBUS_SERVER_CHANNEL_INPUT_REGISTERS, MODBUS_DEMO_INPUT_REGISTERS_ADDRESS,
      MODBUS_DEMO_INPUT_REGISTERS_NB, FALSE);

  return modbus_memory_default_create(bits, input_bits, registers, input_registers);
}

modbus_memory_t* modbus_memory_default_create(modbus_server_channel_t* bits,
                                              modbus_server_channel_t* input_bits,
                                              modbus_server_channel_t* registers,
                                              modbus_server_channel_t* input_registers) {
  modbus_memory_default_t* memory = TKMEM_ZALLOC(modbus_memory_default_t);
  return_value_if_fail(memory != NULL, NULL);

  memory->memory.read_bits = modbus_memory_default_read_bits;
  memory->memory.read_input_bits = modbus_memory_default_read_input_bits;
  memory->memory.read_registers = modbus_memory_default_read_registers;
  memory->memory.read_input_registers = modbus_memory_default_read_input_registers;
  memory->memory.write_bit = modbus_memory_default_write_bit;
  memory->memory.write_bits = modbus_memory_default_write_bits;
  memory->memory.write_register = modbus_memory_default_write_register;
  memory->memory.write_registers = modbus_memory_default_write_registers;
  memory->memory.destroy = modbus_memory_default_destroy;

  memory->bits = bits;
  memory->input_bits = input_bits;
  memory->registers = registers;
  memory->input_registers = input_registers;

  log_debug("-------------------------------------------------\n");
  if (bits != NULL) {
    log_debug("%s: start=%u length=%u bytes=%u writable=%d\n", bits->name, bits->start,
              bits->length, bits->bytes, bits->writable);
  }

  if (input_bits != NULL) {
    log_debug("%s: start=%u length=%u bytes=%u writable=%d\n", input_bits->name, input_bits->start,
              input_bits->length, input_bits->bytes, input_bits->writable);
  }

  if (registers != NULL) {
    log_debug("%s: start=%u length=%u bytes=%u writable=%d\n", registers->name, registers->start,
              registers->length, registers->bytes, registers->writable);
  }

  if (input_registers != NULL) {
    log_debug("%s: start=%u length=%u bytes=%u writable=%d\n", input_registers->name,
              input_registers->start, input_registers->length, input_registers->bytes,
              input_registers->writable);
  }
  log_debug("-------------------------------------------------\n");

  memory->emitter = emitter_create();

  return (modbus_memory_t*)memory;
}

modbus_memory_t* modbus_memory_default_create_with_conf(conf_node_t* node) {
  modbus_server_channel_t* bits = NULL;
  modbus_server_channel_t* input_bits = NULL;
  modbus_server_channel_t* registers = NULL;
  modbus_server_channel_t* input_registers = NULL;
  conf_node_t* iter = conf_node_get_first_child(node);
  return_value_if_fail(iter != NULL, NULL);

  while (iter != NULL) {
    modbus_server_channel_t* channel = modbus_server_channel_create_with_conf(iter);

    if (channel != NULL) {
      if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_BITS)) {
        bits = channel;
      } else if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_INPUT_BITS)) {
        input_bits = channel;
      } else if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_REGISTERS)) {
        registers = channel;
      } else if (tk_str_eq(channel->name, MODBUS_SERVER_CHANNEL_INPUT_REGISTERS)) {
        input_registers = channel;
      } else {
        modbus_server_channel_destroy(channel);
        log_debug("invalid channel name: %s\n", channel->name);
      }
    }

    iter = iter->next;
  }

  return modbus_memory_default_create(bits, input_bits, registers, input_registers);
}

modbus_memory_default_t* modbus_memory_default_cast(modbus_memory_t* memory) {
  return_value_if_fail(memory != NULL, NULL);

  if (memory->read_bits == modbus_memory_default_read_bits) {
    return (modbus_memory_default_t*)memory;
  }

  return NULL;
}

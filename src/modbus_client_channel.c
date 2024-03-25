/**
 * File:   modbus_client_channel.c
 * Author: AWTK Develop Team
 * Brief:  modbus_client_channel
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

#include "tkc/mem.h"
#include "modbus_client_channel.h"

static ret_t modbus_client_channel_init(modbus_client_channel_t* channel);
static ret_t modbus_client_channel_load(modbus_client_channel_t* channel, conf_node_t* node);

static ret_t modbus_client_clear_buffer_if_fail(modbus_client_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);
  if (!channel->keep_last_value_if_read_failed) {
    if (channel->read_buffer != NULL) {
      memset(channel->read_buffer, 0, channel->read_buffer_length);
    }
  }

  return RET_OK;
}

modbus_client_channel_t* modbus_client_channel_create(conf_node_t* node) {
  modbus_client_channel_t* channel = TKMEM_ZALLOC(modbus_client_channel_t);
  return_value_if_fail(channel != NULL, NULL);

  if (modbus_client_channel_load(channel, node) != RET_OK) {
    modbus_client_channel_destroy(channel);
    return NULL;
  }

  if (modbus_client_channel_init(channel) != RET_OK) {
    modbus_client_channel_destroy(channel);
    return NULL;
  }

  return channel;
}

ret_t modbus_client_channel_read(modbus_client_channel_t* channel) {
  ret_t ret = RET_FAIL;
  modbus_client_t* client = NULL;
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);
  if (channel->client == NULL) {
    modbus_client_clear_buffer_if_fail(channel);
  }
  client = channel->client;
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  switch (channel->access_type) {
    case MODBUS_FC_READ_COILS: {
      /*如果长度超过了最大长度(MODBUS_MAX_READ_BITS)，需要分包读取*/
      uint32_t offset = 0;
      uint32_t length = channel->bits_length;

      while (length > 0) {
        uint32_t len = length > MODBUS_MAX_READ_BITS ? MODBUS_MAX_READ_BITS : length;
        ret = modbus_client_read_bits(client, channel->read_offset + offset, len,
                                      channel->bits_buffer);
        if (ret != RET_OK) {
          break;
        }
        tk_bits_data_from_bytes_data(channel->read_buffer + offset / 8, tk_bits_to_bytes(len),
                                     channel->bits_buffer, len);
        offset += len;
        length -= len;
      }
      break;
    }
    case MODBUS_FC_READ_DISCRETE_INPUTS: {
      /*如果长度超过了最大长度(MODBUS_MAX_READ_BITS)，需要分包读取*/
      uint32_t offset = 0;
      uint32_t length = channel->bits_length;
      while (length > 0) {
        uint32_t len = length > MODBUS_MAX_READ_BITS ? MODBUS_MAX_READ_BITS : length;
        ret = modbus_client_read_input_bits(client, channel->read_offset + offset, len,
                                            channel->bits_buffer);
        if (ret != RET_OK) {
          break;
        }
        tk_bits_data_from_bytes_data(channel->read_buffer + offset / 8, tk_bits_to_bytes(len),
                                     channel->bits_buffer, len);
        offset += len;
        length -= len;
      }
      break;
    }
    case MODBUS_FC_READ_HOLDING_REGISTERS: {
      /*如果长度超过了最大长度(MODBUS_MAX_READ_REGISTERS)，需要分包读取*/
      uint32_t offset = 0;
      uint32_t length = channel->read_buffer_length / sizeof(uint16_t);

      while (length > 0) {
        uint32_t len = length > MODBUS_MAX_READ_REGISTERS ? MODBUS_MAX_READ_REGISTERS : length;
        ret = modbus_client_read_registers(
            client, channel->read_offset + offset, len,
            (uint16_t*)(channel->read_buffer + offset * sizeof(uint16_t)));
        if (ret != RET_OK) {
          break;
        }
        offset += len;
        length -= len;
      }
      break;
    }
    case MODBUS_FC_READ_INPUT_REGISTERS: {
      /*如果长度超过了最大长度(MODBUS_MAX_READ_REGISTERS)，需要分包读取*/
      uint32_t offset = 0;
      uint32_t length = channel->read_buffer_length / sizeof(uint16_t);

      while (length > 0) {
        uint32_t len = length > MODBUS_MAX_READ_REGISTERS ? MODBUS_MAX_READ_REGISTERS : length;
        ret = modbus_client_read_input_registers(
            client, channel->read_offset + offset, len,
            (uint16_t*)(channel->read_buffer + offset * sizeof(uint16_t)));
        if (ret != RET_OK) {
          break;
        }
        offset += len;
        length -= len;
      }
      break;
    }
    default: {
      ret = RET_NOT_IMPL;
    }
  }

  if (ret == RET_OK) {
    channel->read_ok_count++;
  } else {
    channel->read_fail_count++;
    modbus_client_clear_buffer_if_fail(channel);
  }

  return ret;
}

ret_t modbus_client_channel_write(modbus_client_channel_t* channel) {
  ret_t ret = RET_FAIL;
  modbus_client_t* client = NULL;
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);
  client = channel->client;
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  switch (channel->access_type) {
    case MODBUS_FC_WRITE_SINGLE_COIL: {
      ret = modbus_client_write_bit(client, channel->write_offset, channel->write_buffer[0] & 0x01);
      break;
    }
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER: {
      ret = modbus_client_write_register(client, channel->write_offset,
                                         *(uint16_t*)(channel->write_buffer));
      break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_COILS: {
      /*如果长度超过了最大长度(MODBUS_MAX_WRITE_BITS)，需要分包发送*/
      uint32_t offset = 0;
      uint32_t length = channel->bits_length;

      while (length > 0) {
        uint32_t len = length > MODBUS_MAX_WRITE_BITS ? MODBUS_MAX_WRITE_BITS : length;
        tk_bits_data_to_bytes_data(channel->write_buffer + offset / 8, tk_bits_to_bytes(len),
                                   channel->bits_buffer, len);
        ret = modbus_client_write_bits(client, channel->write_offset + offset, len,
                                       channel->bits_buffer);
        if (ret != RET_OK) {
          break;
        }
        offset += len;
        length -= len;
      }
      break;
    }
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
      /*如果长度超过了最大长度(MODBUS_MAX_WRITE_REGISTERS)，需要分包发送*/
      uint32_t offset = 0;
      uint32_t length = channel->write_buffer_length / sizeof(uint16_t);
      
      while (length > 0) {
        uint32_t len = length > MODBUS_MAX_WRITE_REGISTERS ? MODBUS_MAX_WRITE_REGISTERS : length;
        ret = modbus_client_write_registers(
            client, channel->write_offset + offset, len,
            (uint16_t*)(channel->write_buffer + offset * sizeof(uint16_t)));
        if (ret != RET_OK) {
          break;
        }
        offset += len;
        length -= len;
      }
      break;
    }
    default: {
      ret = RET_NOT_IMPL;
    }
  }

  if (ret == RET_OK) {
    channel->write_ok_count++;
  } else {
    channel->write_fail_count++;
  }

  return ret;
}

static ret_t modbus_client_channel_init(modbus_client_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  if (channel->access_type == MODBUS_FC_WRITE_SINGLE_COIL) {
    channel->write_buffer_length = 1;
  } else if (channel->access_type == MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER) {
    channel->write_buffer_length = 1;
  }

  if (channel->access_type == MODBUS_FC_READ_COILS ||
      channel->access_type == MODBUS_FC_READ_DISCRETE_INPUTS) {
    channel->bits_length = channel->read_buffer_length;
    channel->bits_buffer = TKMEM_ALLOC(channel->bits_length);
    return_value_if_fail(channel->bits_buffer != NULL, RET_OOM);

    assert(channel->bits_length > 0);
    memset(channel->bits_buffer, 0, channel->bits_length);
  }

  if (channel->access_type == MODBUS_FC_WRITE_MULTIPLE_COILS ||
      channel->access_type == MODBUS_FC_WRITE_SINGLE_COIL) {
    channel->bits_length = channel->write_buffer_length;
    channel->bits_buffer = TKMEM_ALLOC(channel->bits_length);
    return_value_if_fail(channel->bits_buffer != NULL, RET_OOM);

    assert(channel->bits_length > 0);
    memset(channel->bits_buffer, 0, channel->bits_length);
  }

  if (channel->read_buffer_length > 0) {
    uint32_t length = channel->read_buffer_length;

    if (channel->access_type == MODBUS_FC_READ_COILS ||
        channel->access_type == MODBUS_FC_READ_DISCRETE_INPUTS) {
      length = tk_bits_to_bytes(channel->read_buffer_length);
    } else {
      length = channel->read_buffer_length * sizeof(uint16_t);
    }

    channel->read_buffer = TKMEM_ALLOC(length);
    channel->read_buffer_length = length;
    return_value_if_fail(channel->read_buffer != NULL, RET_OOM);

    memset(channel->read_buffer, 0, length);
  }

  if (channel->write_buffer_length > 0) {
    uint32_t length = channel->write_buffer_length;

    if (channel->access_type == MODBUS_FC_WRITE_MULTIPLE_COILS ||
        channel->access_type == MODBUS_FC_WRITE_SINGLE_COIL) {
      length = tk_bits_to_bytes(channel->write_buffer_length);
    } else {
      length = channel->write_buffer_length * sizeof(uint16_t);
    }

    channel->write_buffer = TKMEM_ALLOC(length);
    channel->write_buffer_length = length;
    return_value_if_fail(channel->write_buffer != NULL, RET_OOM);

    memset(channel->write_buffer, 0, length);
  }

  channel->mutex = tk_mutex_create();
  return_value_if_fail(channel->mutex != NULL, RET_OOM);

  return RET_OK;
}

ret_t modbus_client_channel_set_client(modbus_client_channel_t* channel, modbus_client_t* client) {
  return_value_if_fail(channel != NULL && client != NULL, RET_BAD_PARAMS);

  channel->client = client;

  return RET_OK;
}

bool_t modbus_client_channel_need_update(modbus_client_channel_t* modbus_channel,
                                         uint64_t current_time) {
  return_value_if_fail(modbus_channel != NULL, FALSE);

  return modbus_channel->next_update_time <= current_time;
}

ret_t modbus_client_channel_update(modbus_client_channel_t* channel, uint64_t current_time) {
  ret_t ret = RET_OK;
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  if (channel->client == NULL) {
    modbus_client_clear_buffer_if_fail(channel);
  }

  return_value_if_fail(channel->client != NULL, RET_BAD_PARAMS);

  if (channel->next_update_time > current_time) {
    return RET_NOT_MODIFIED;
  }

  switch (channel->access_type) {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
      ret = modbus_client_channel_read(channel);
      break;
    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER:
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS:
      ret = modbus_client_channel_write(channel);
      break;
    default:
      break;
  }

  channel->next_update_time = time_now_ms() + channel->update_interval;

  return ret;
}

ret_t modbus_client_channel_lock(modbus_client_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  return tk_mutex_lock(channel->mutex);
}

ret_t modbus_client_channel_unlock(modbus_client_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  return tk_mutex_unlock(channel->mutex);
}

ret_t modbus_client_channel_set_name(modbus_client_channel_t* channel, const char* name) {
  return_value_if_fail(channel != NULL && name != NULL, RET_BAD_PARAMS);

  channel->name = tk_str_copy(channel->name, name);

  return RET_OK;
}

static ret_t modbus_client_channel_load(modbus_client_channel_t* channel, conf_node_t* node) {
  return_value_if_fail(channel != NULL && node != NULL, RET_BAD_PARAMS);

  channel->name = tk_strdup(conf_node_get_child_value_str(node, "name", NULL));
  channel->access_type = conf_node_get_child_value_int32(node, "access_type", 0);
  channel->update_interval = conf_node_get_child_value_int32(node, "update_interval", 0);
  if (channel->update_interval == 0) {
    channel->update_interval = conf_node_get_child_value_int32(node, "cycle_time", -1);
  }
  channel->unit_id = conf_node_get_child_value_int32(node, "unit_id", 0);

  {
    const char* error_handling = NULL;
    conf_node_t* read_info = conf_node_find_child(node, "read");
    if (read_info != NULL) {
      channel->read_offset = conf_node_get_child_value_int32(read_info, "offset", 0);
      channel->read_buffer_length = conf_node_get_child_value_int32(read_info, "length", 0);
      error_handling = conf_node_get_child_value_str(read_info, "error_handling", NULL);
      if (error_handling == NULL || tk_str_eq(error_handling, "keep_last_value")) {
        channel->keep_last_value_if_read_failed = TRUE;
      } else {
        channel->keep_last_value_if_read_failed = FALSE;
      }
    }
  }

  {
    conf_node_t* write_info = conf_node_find_child(node, "write");
    if (write_info != NULL) {
      channel->write_offset = conf_node_get_child_value_int32(write_info, "offset", 0);
      channel->write_buffer_length = conf_node_get_child_value_int32(write_info, "length", 0);
    }
  }

  return RET_OK;
}

ret_t modbus_client_channel_destroy(modbus_client_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(channel->name);
  TKMEM_FREE(channel->bits_buffer);
  TKMEM_FREE(channel->read_buffer);
  TKMEM_FREE(channel->write_buffer);
  tk_mutex_destroy(channel->mutex);

  TKMEM_FREE(channel);

  return RET_OK;
}

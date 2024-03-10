/**
 * File:   modbus_server_channel.c
 * Author: AWTK Develop Team
 * Brief:  modbus_server_channel
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
 * 2024-3-10 Li XianJing <lixianjing@zlg.cn> created.
 *
 */

#include "tkc/mem.h"
#include "modbus_server_channel.h"

static ret_t modbus_server_channel_init(modbus_server_channel_t* channel) {
  uint32_t bytes = 0;
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  if (strstr(channel->name, MODBUS_SERVER_CHANNEL_BITS) != NULL ||
      strstr(channel->name, MODBUS_SERVER_CHANNEL_INPUT_BITS) != NULL) {
    bytes = tk_bits_to_bytes(channel->length);
  } else {
    bytes = channel->length * sizeof(uint16_t);
  }
  return_value_if_fail(bytes > 0, RET_BAD_PARAMS);

  channel->bytes = bytes;
  channel->data = TKMEM_ALLOC(bytes);
  return_value_if_fail(channel->data != NULL, RET_OOM);

  memset(channel->data, 0, bytes);

  return RET_OK;
}

ret_t modbus_server_channel_destroy(modbus_server_channel_t* channel) {
  return_value_if_fail(channel != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(channel->data);
  TKMEM_FREE(channel);

  return RET_OK;
}

modbus_server_channel_t* modbus_server_channel_create(const char* name, uint32_t start,
                                                      uint32_t length, bool_t writable) {
  modbus_server_channel_t* channel = TKMEM_ZALLOC(modbus_server_channel_t);
  return_value_if_fail(channel != NULL, NULL);

  channel->name = tk_strdup(name);
  channel->start = start;
  channel->length = length;
  channel->writable = writable;

  modbus_server_channel_init(channel);

  return channel;
}

modbus_server_channel_t* modbus_server_channel_create_with_conf(conf_node_t* node) {
  modbus_server_channel_t* channel = TKMEM_ZALLOC(modbus_server_channel_t);
  return_value_if_fail(node != NULL, NULL);
  return_value_if_fail(channel != NULL, NULL);

  channel->name = tk_strdup(conf_node_get_child_value_str(node, "name", NULL));
  channel->start = conf_node_get_child_value_int32(node, "start", 0);
  channel->length = conf_node_get_child_value_int32(node, "length", 0);
  channel->writable = conf_node_get_child_value_bool(node, "writable", FALSE);

  modbus_server_channel_init(channel);

  return channel;
}

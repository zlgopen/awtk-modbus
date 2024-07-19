/**
 * File:   modbus_client_channel.h
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

#include "tkc.h"
#include "modbus_client.h"
#include "conf_io/conf_node.h"

#ifndef MODBUS_CLIENT_CHANNEL
#define MODBUS_CLIENT_CHANNEL

BEGIN_C_DECLS

/**
 * @class modbus_client_channel_t
 * modbus_client_channel
 */
typedef struct _modbus_client_channel_t {
  /**
   * @property {char*} name
   * @annotation ["readable"]
   * 名称。
   */
  char* name;
  /**
   * @property {uint8_t} access_type
   * @annotation ["readable"]
   * 访问类型。
   */
  uint8_t access_type;
  /**
   * @property {uint8_t} unit_id
   * @annotation ["readable"]
   * RTU模式从站 ID。
   */
  uint8_t unit_id;
  /**
   * @property {uint32_t} read_offset
   * @annotation ["readable"]
   * 读取偏移。
   */
  uint32_t read_offset;
  /**
   * @property {uint32_t} write_offset
   * @annotation ["readable"]
   * 写入偏移。
   */
  uint32_t write_offset;
  /** 
   * @property {uint8_t*} read_buffer
   * @annotation ["readable"]
   * 读取缓冲区。
   */
  uint8_t* read_buffer;
  /** 
   * @property {uint8_t*} write_buffer
   * @annotation ["readable"]
   * 写入缓冲区。
   */
  uint8_t* write_buffer;
  /** 
   * @property {uint32_t} read_buffer_length
   * @annotation ["readable"]
   * 读取缓冲区长度。
   */
  uint32_t read_buffer_length;
  /**
   * @property {uint32_t} write_buffer_length
   * @annotation ["readable"]
   * 写入缓冲区长度。
   */
  uint32_t write_buffer_length;
  /**
   * @property {uint32_t} update_interval
   * @annotation ["readable"]
   * 更新间隔。
  */
  uint32_t update_interval;
  /**
   * @property {uint64_t} next_update_time
   * @annotation ["readable"]
   * 下次更新时间。
   */
  uint64_t next_update_time;
  /**
   * @property {bool_t} keep_last_value_if_read_failed
   * @annotation ["readable"]
   * 读取失败时是否保留上次的值。
   */
  bool_t keep_last_value_if_read_failed;
  /**
   * @property {modbus_client_t*} client
   * @annotation ["readable"]
   * 客户端。
   */
  modbus_client_t* client;
  /**
   * @property {uint32_t} read_ok_count
   * @annotation ["readable"]
   * 读取成功次数。
   */
  uint32_t read_ok_count;
  /**
   * @property {uint32_t} read_fail_count
   * @annotation ["readable"]
   * 读取失败次数。
   */
  uint32_t read_fail_count;
  /**
   * @property {uint32_t} write_ok_count
   * @annotation ["readable"]
   * 写入成功次数。
   */
  uint32_t write_ok_count;
  /**
   * @property {uint32_t} write_fail_count
   * @annotation ["readable"]
   * 写入失败次数。
   */
  uint32_t write_fail_count;

  /*private*/
  /*对于bits操作，在bits_buffer中，每个bit占一个字节*/
  uint8_t* bits_buffer;
  uint32_t bits_length;
  tk_mutex_t* mutex;
} modbus_client_channel_t;

/**
 * @method modbus_client_channel_create
 * 创建modbus_client_channel对象。
 * @param {conf_node_t*} node 配置节点。
 * @return {modbus_client_channel_t*} 返回对象。
 */
modbus_client_channel_t* modbus_client_channel_create(conf_node_t* node);

/**
 * @method modbus_client_channel_need_update
 * 是否需要更新数据。
 * @param {modbus_client_channel_t*} modbus_channel 对象。
 * @param {uint64_t} current_time 当前时间。
 * 
 * @return {bool_t} 返回TRUE表示需要更新，否则表示不需要更新。
 */
bool_t modbus_client_channel_need_update(modbus_client_channel_t* modbus_channel, uint64_t current_time);

/**
 * @method modbus_client_channel_set_client
 * 设置客户端。
 * @param {modbus_client_channel_t*} channel 对象。
 * @param {modbus_client_t*} client 客户端。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
*/
ret_t modbus_client_channel_set_client(modbus_client_channel_t* channel, modbus_client_t* client);

/**
 * @method modbus_client_channel_set_name
 * 设置名称。
 * @param {modbus_client_channel_t*} channel 对象。
 * @param {const char*} name 名称。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_set_name(modbus_client_channel_t* channel, const char* name);

/**
 * @method modbus_client_channel_set_unit_id
 * 设置 unit_id。
 * @param {modbus_client_channel_t*} channel 对象。
 * @param {uint8_t} unit_id unit_id。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_set_unit_id(modbus_client_channel_t* channel, uint8_t unit_id);

/**
 * @method modbus_client_channel_read
 * 读取数据。
 * @param {modbus_client_channel_t*} modbus_channel 对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_read(modbus_client_channel_t* modbus_channel);

/**
 * @method modbus_client_channel_write
 * 写入数据。
 * @param {modbus_client_channel_t*} modbus_channel 对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_write(modbus_client_channel_t* modbus_channel);

/**
 * @method modbus_client_channel_update
 * 更新数据(读写数据)。
 * @param {modbus_client_channel_t*} modbus_channel 对象。
 * @param {uint64_t} current_time 当前时间。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_update(modbus_client_channel_t* modbus_channel, uint64_t current_time);

/**
 * @method modbus_client_channel_lock
 * 锁定modbus_client_channel对象。
 * @param {modbus_client_channel_t*} channel 对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_lock(modbus_client_channel_t* channel);

/**
 * @method modbus_client_channel_unlock
 * 解锁modbus_client_channel对象。
 * @param {modbus_client_channel_t*} channel 对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_unlock(modbus_client_channel_t* channel);

/**
 * @method modbus_client_channel_destroy
 * 销毁modbus_client_channel对象。
 * @param {modbus_client_channel_t*} channel 对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_client_channel_destroy(modbus_client_channel_t* channel);

END_C_DECLS

#endif /*MODBUS_CLIENT_CHANNEL*/

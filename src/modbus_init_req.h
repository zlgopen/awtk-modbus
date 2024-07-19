/**
 * File:   modbus_init_req.h
 * Author: AWTK Develop Team
 * Brief:  modbus init req
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

#include "modbus_client.h"
#include "conf_io/conf_node.h"

#ifndef MODBUS_INIT_REQ
#define MODBUS_INIT_REQ

BEGIN_C_DECLS

/**
 * @class modbus_init_req_t
 * 初始化数据。
 */
typedef struct _modbus_init_req_t {
  /**
   * @property {uint8_t} access_type
   * @annotation ["readable"]
   * 访问类型。
   */
  uint8_t access_type;

  /**
   * @property {uint8_t} unit_id
   * @annotation ["readable"]
   * slave ID。
   */
  uint8_t unit_id;
  
  /**
   * @property {uint32_t} offset
   * @annotation ["readable"]
   * 写入偏移。
   */
  uint32_t offset;

  /**
   * @property {uint32_t} length
   * @annotation ["readable"]
   * 写入大小。
   */
  uint32_t length;

  /**
   * @property {uint16_t} value
   * @annotation ["readable"]
   * 写入大小。
   */
  uint16_t value;
} modbus_init_req_t;

/**
 * @method modbus_init_req_create
 * 创建modbus_init_req对象。
 * @param {conf_node_t*} node 配置节点。
 *
 * @return {modbus_init_req_t*} 返回对象。
 */
modbus_init_req_t* modbus_init_req_create(conf_node_t* node);

/**
 * @method modbus_init_req_request
 * 请求初始化数据。
 *
 * @param {modbus_init_req_t*} init 对象。
 * @param {modbus_client_t*} client 客户端。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_init_req_request(modbus_init_req_t* init, modbus_client_t* client);

/**
 * @method modbus_init_req_destroy
 * 销毁对象。
 *
 * @param {modbus_init_req_t*} init 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_init_req_destroy(modbus_init_req_t* init);

END_C_DECLS

#endif /*MODBUS_INIT_REQ*/

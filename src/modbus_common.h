/**
 * File:   modbus_common.h
 * Author: AWTK Develop Team
 * Brief:  modbus_common
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
 * 2023-09-29 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_MODBUS_COMMON_H
#define TK_MODBUS_COMMON_H

#include "modbus_types_def.h"

BEGIN_C_DECLS

/**
 * @class modbus_common_t
 * 
 * modbus common
 */
typedef struct _modbus_common_t {
  /**
   * @property {modbus_proto_t} proto
   * 协议。
   */
  modbus_proto_t proto;
  /**
   * @property {wbuffer_t*} wbuffer
   * wbuffer。
   */
  wbuffer_t* wbuffer;
  /**
   * @property {tk_iostream_t*} io
   * io对象。
   */
  tk_iostream_t* io;
  /**
   * @property {uint32_t} write_timeout
   * 写超时。
   */
  uint32_t write_timeout;
  /**
   * @property {uint32_t} read_timeout
   * 读超时。
   */
  uint32_t read_timeout;
  /**
   * @property {uint16_t} slave
   * 从站地址。
   */
  uint16_t slave;
  /**
   * @property {uint16_t} transaction_id
   * 事务ID。
   */
  uint16_t transaction_id;
  /**
   * @property {modbus_exeption_code_t} last_exception_code
   * 最后一次的错误码。
  */
  modbus_exeption_code_t last_exception_code;
} modbus_common_t;

/**
 * @method modbus_common_init
 * 初始化modbus_common_t对象。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {tk_iostream_t*} io io对象。
 * @param {modbus_proto_t} proto 协议。
 * @param {wbuffer_t*} wb wbuffer对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_init(modbus_common_t* common, tk_iostream_t* io, modbus_proto_t proto, wbuffer_t* wb);

/**
 * @method modbus_common_send_read_bits_req
 * 发送读取bits请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} func_code 功能码。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_read_bits_req(modbus_common_t* common, uint16_t func_code, uint16_t addr,
                                       uint16_t count);

/**
 * @method modbus_common_recv_read_bits_resp
 * 接收读取bits响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} func_code 功能码。
 * @param {uint8_t*} buffer 读取的数据。
 * @param {uint16_t*} count 读取的个数(即作输入又作输出)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_read_bits_resp(modbus_common_t* common, uint16_t func_code,
                                        uint8_t* buffer, uint16_t* count);

/**
 * @method modbus_common_send_read_registers_req
 * 发送读取registers请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} func_code 功能码。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_read_registers_req(modbus_common_t* common, uint16_t func_code,
                                            uint16_t addr, uint16_t count);

/**
 * @method modbus_common_recv_read_registers_resp
 * 接收读取registers响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} func_code 功能码。
 * @param {uint16_t*} buffer 读取的数据。
 * @param {uint16_t*} count 读取的个数(即作输入又作输出)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_read_registers_resp(modbus_common_t* common, uint16_t func_code,
                                             uint16_t* buffer, uint16_t* count);

/**
 * @method modbus_common_send_write_bit_req
 * 发送写入bit请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} addr 地址。
 * @param {uint8_t} value 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_write_bit_req(modbus_common_t* common, uint16_t addr, uint8_t value);

/**
 * @method modbus_common_recv_write_bit_resp
 * 接收写入bit响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_write_bit_resp(modbus_common_t* common);

/**
 * @method modbus_common_send_write_register_req
 * 发送写入register请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} value 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_write_register_req(modbus_common_t* common, uint16_t addr, uint16_t value);

/**
 * @method modbus_common_recv_write_register_resp
 * 接收写入register响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_write_register_resp(modbus_common_t* common);

/**
 * @method modbus_common_send_write_bits_req
 * 发送写入bits请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {const uint8_t*} buff 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_write_bits_req(modbus_common_t* common, uint16_t addr, uint16_t count,
                                        const uint8_t* buff);

/**
 * @method modbus_common_recv_write_bits_resp
 * 接收写入bits响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_write_bits_resp(modbus_common_t* common);

/**
 * @method modbus_common_send_write_registers_req
 * 发送写入registers请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint16_t} addr 地址。
 * @param {uint16_t} count 个数。
 * @param {const uint16_t*} buff 值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_write_registers_req(modbus_common_t* common, uint16_t addr, uint16_t count,
                                             const uint16_t* buff);

/**
 * @method modbus_common_recv_write_registers_resp
 * 接收写入registers响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_write_registers_resp(modbus_common_t* common);

/**
 * @method modbus_common_get_last_exception_code
 * 获取最后一次的错误码。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {modbus_exeption_code_t} 返回最后一次的错误码。
 */
modbus_exeption_code_t modbus_common_get_last_exception_code(modbus_common_t* common);

/**
 * @method modbus_common_get_last_exception_str
 * 获取最后一次的错误码字符串。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {const char*} 返回最后一次的错误码字符串。
 */
const char* modbus_common_get_last_exception_str(modbus_common_t* common);

/**
 * @method modbus_common_deinit
 * 释放modbus_common_t对象。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_deinit(modbus_common_t* common);

/* for service side */
/**
 * @method modbus_common_recv_req
 * 解包请求。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {modbus_req_data_t*} req_data 请求数据。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_recv_req(modbus_common_t* common, modbus_req_data_t* req_data);

/**
 * @method modbus_common_send_resp
 * 发送响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {modbus_resp_data_t*} resp_data 响应数据。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_resp(modbus_common_t* common, modbus_resp_data_t* resp_data);

/**
 * @method modbus_common_send_exception_resp
 * 发送异常响应。
 * @param {modbus_common_t*} common modbus_common_t对象。
 * @param {uint8_t} func_code 功能码。
 * @param {modbus_exeption_code_t} code 异常码。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_common_send_exception_resp(modbus_common_t* common, uint8_t func_code,
                                        modbus_exeption_code_t code);

#define MODBUS_COMMON(obj) ((obj) != NULL ? &((obj)->common) : NULL)

END_C_DECLS

#endif /*TK_MODBUS_COMMON_H*/

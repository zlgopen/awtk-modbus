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

#ifndef TK_MODBUS_TYPES_DEF_H
#define TK_MODBUS_TYPES_DEF_H

#include "tkc.h"

BEGIN_C_DECLS

/**
 * @enum modbus_function_code_t
 * @prefix MODBUS_FC_ 
 * MODBUS 功能码
 */
typedef enum _modbus_function_code_t {
  /**
   * @const MODBUS_FC_READ_COILS
   * 读取COILS。
   */
  MODBUS_FC_READ_COILS = 1,
  /**
   * @const MODBUS_FC_READ_DISCRETE_INPUTS
   * 读取DISCRETE_INPUTS。
   */
  MODBUS_FC_READ_DISCRETE_INPUTS = 2,
  /**
   * @const MODBUS_FC_READ_HOLDING_REGISTERS
   * 读取HOLDING_REGISTERS。
   */
  MODBUS_FC_READ_HOLDING_REGISTERS = 3,
  /**
   * @const MODBUS_FC_READ_INPUT_REGISTERS
   * 读取INPUT_REGISTERS。
   */
  MODBUS_FC_READ_INPUT_REGISTERS = 4,
  /**
   * @const MODBUS_FC_WRITE_SINGLE_COIL
   * 写入单个COIL。
   */
  MODBUS_FC_WRITE_SINGLE_COIL = 5,
  /**
   * @const MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER
   * 写入单个HOLDING_REGISTER。
   */
  MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER = 6,
  /**
   * @const MODBUS_FC_WRITE_MULTIPLE_COILS
   * 写入多个COILS。
   */
  MODBUS_FC_WRITE_MULTIPLE_COILS = 15,
  /**
   * @const MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS
   * 写入多个HOLDING_REGISTERS。
   */
  MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS = 16
} modbus_function_code_t;

/**
 * @enum modbus_proto_t
 * @prefix MODBUS_PROTO_
 * MODBUS 协议类型
 */
typedef enum _modbus_proto_t {
  /**
   * @const MODBUS_PROTO_RTU
   * RTU 协议。
   */
  MODBUS_PROTO_RTU = 0,
  /**
   * @const MODBUS_PROTO_TCP
   * TCP 协议。
   */
  MODBUS_PROTO_TCP = 1
} modbus_proto_t;

/**
 * @enum modbus_exeption_code_t
 * @prefix MODBUS_EXCEPTION_
 * MODBUS 异常码
 */
typedef enum _modbus_exeption_code_t {
  /**
   * @const MODBUS_EXCEPTION_ILLEGAL_FUNCTION
   * 非法功能。
   */
  MODBUS_EXCEPTION_ILLEGAL_FUNCTION = 1,
  /**
   * @const MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS
   * 非法数据地址。
   */
  MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS = 2,
  /**
   * @const MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE
   * 非法数据值。
   */
  MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE = 3,
  /**
   * @const MODBUS_EXCEPTION_SERVER_DEVICE_FAILURE
   * 从站设备故障。
   */
  MODBUS_EXCEPTION_SERVER_DEVICE_FAILURE = 4,
  /**
   * @const MODBUS_EXCEPTION_ACKNOWLEDGE
   * 确认。
   */
  MODBUS_EXCEPTION_ACKNOWLEDGE = 5,
  /**
   * @const MODBUS_EXCEPTION_SERVER_DEVICE_BUSY
   * 从站设备忙。
   */
  MODBUS_EXCEPTION_SERVER_DEVICE_BUSY = 6,
  /**
   * @const MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE
   * 否认。
   */
  MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE = 7,
  /**
   * @const MODBUS_EXCEPTION_MEMORY_PARITY_ERROR
   * 内存奇偶校验错误。
   */
  MODBUS_EXCEPTION_MEMORY_PARITY_ERROR = 8,
  /**
   * @const MODBUS_EXCEPTION_GATEWAY_PATH_UNAVAILABLE
   * 网关路径不可用。
   */
  MODBUS_EXCEPTION_GATEWAY_PATH_UNAVAILABLE = 10,
  /**
   * @const MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND
   * 网关目标设备无响应。
   */
  MODBUS_EXCEPTION_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 11
} modbus_exeption_code_t;

/**
 * @class modbus_resp_header_t
 * resp header
*/
typedef struct _modbus_resp_header_t {
  /**
     * @property {uint8_t} slave
     * 从站地址。
     */
  uint8_t slave;
  /**
     * @property {uint8_t} func_code
     * 功能码。
     */
  uint8_t func_code;
  /**
     * @property {uint8_t} data_len
     * 数据长度。
     */
  uint8_t data_len;
} modbus_resp_header_t;

/**
 * @class modbus_req_data_t
 * modbus data
*/
typedef struct _modbus_req_data_t {
  /**
   * @property {uint8_t} func_code
   * 功能码。
   */
  uint8_t func_code;
  /**
   * @property {uint8_t} slave
   * 从站地址。
   */
  uint8_t slave;
  /**
   * @property {uint16_t} addr
    * 地址。
    */
  uint16_t addr;
  /**
    * @property {uint16_t} count
    * 个数。
    */
  uint16_t count;

  /**
   * @property {uint8_t} bytes
   * 数据长度。
   */
  uint8_t bytes;

  /**
   * @property {uint8_t*} data
   * 数据。
   */
  uint8_t* data;
} modbus_req_data_t, modbus_resp_data_t;

#pragma pack(push, 1)
/**
 * @class modbus_tcp_header_t
 * modbus tcp header
*/
typedef struct _modbus_tcp_header_t {
  /**
   * @property {uint16_t} transaction_id
   * 事务ID。
   */
  uint16_t transaction_id;
  /**
   * @property {uint16_t} protocol_id
   * 协议ID。
   */
  uint16_t protocol_id;
  /**
    * @property {uint16_t} length
    * 长度。
   */
  uint16_t length;
  /**
   * @property {uint8_t} unit_id
   * 单元ID。
   */
  uint8_t unit_id;
  /**
   * @property {uint8_t} func_code
   * 功能码。
   */
  uint8_t func_code;
} modbus_tcp_header_t;

/**
 * @class modbus_rtu_header_t
 * modbus rtu header
*/
typedef struct _modbus_rtu_header_t {
  /**
   * @property {uint8_t} slave
   * slave ID。
   */
  uint8_t slave;
  /**
   * @property {uint8_t} func_code
   * 功能码。
   */
  uint8_t func_code;
} modbus_rtu_header_t;
#pragma pack(pop)

#define MODBUS_MAX_PDU_SIZE 256
#define MODBUS_MAX_READ_BITS 2000
#define MODBUS_MAX_WRITE_BITS 1968
#define MODBUS_MAX_READ_REGISTERS 125
#define MODBUS_MAX_WRITE_REGISTERS 123

#ifndef MODBUS_WRITE_TIMEOUT
#define MODBUS_WRITE_TIMEOUT 500 /*0.5s*/
#endif                           /*MODBUS_WRITE_TIMEOUT*/

#ifndef MODBUS_READ_TIMEOUT
#define MODBUS_READ_TIMEOUT 500 /*0.5*/
#endif                                 /*MODBUS_READ_TIMEOUT*/

/*for demo app*/
#define MODBUS_DEMO_BITS_ADDRESS 0x130
#define MODBUS_DEMO_BITS_NB 1000

#define MODBUS_DEMO_INPUT_BITS_ADDRESS 0x200
#define MODBUS_DEMO_INPUT_BITS_NB 1000

#define MODBUS_DEMO_REGISTERS_ADDRESS 0x160
#define MODBUS_DEMO_REGISTERS_NB 1000

#define MODBUS_DEMO_INPUT_REGISTERS_ADDRESS 0x300
#define MODBUS_DEMO_INPUT_REGISTERS_NB 1000

#define MODBUS_DEMO_SLAVE_ID 1

#define STR_SCHEMA_RTU_OVER_TCP "rtu+tcp://"
#define STR_SCHEMA_RTU_OVER_UDP "rtu+udp://"

/**
 * 如果485上有多路slave，请定义下面的宏。
 * #define WITH_MULT_SLAVES 1
 */

static uint32_t modbus_bits_to_bytes(uint32_t bits) {
  return (bits + 7) / 8;
}

END_C_DECLS

#endif /*TK_MODBUS_TYPES_DEF_H*/

/**
 * File:   modbus_service_rtu.h
 * Author: AWTK Develop Team
 * Brief:  modbus service rtu
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
 * 2023-10-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_MODBUS_SERVICE_RTU_H
#define TK_MODBUS_SERVICE_RTU_H

#include "modbus_service.h"

BEGIN_C_DECLS

/**
 * @class modbus_service_rtu_t
 * @annotation ["fake"]
 * modbus service rtu
 */

/**
 * @method modbus_service_rtu_start
 * 创建modbus service RTU。
 *
 * 串口URL参数说明：
 * * baudrate {int} 波特率。常用值： 9600, 38400, 57600, 115200。(默认值：115200)
 * * stopbits {string} 停止位。可选值：1, 1.5, 2。(默认值：1)
 * * parity {string} 校验位。可选值：none, odd, even, mark, space。(默认值：none)
 * * flowcontrol {string} 流控。可选值：none, hardware, software。(默认值：none)
 * * bytesize {string} 数据位。可选值：5, 6, 7, 8 。(默认值：8) 
 * 
 * > 示例：
 * * serial://COM1?baudrate=115200
 * * serial://COM2?baudrate=115200&stopbits=1&parity=odd&flowcontrol=soft&bytesize=8
 *
 * @param {event_source_manager_t*} esm 事件管理对象(为NULL则阻塞运行)。
 * @param {modbus_memory_t*} memory 内存对象。
 * @param {const char*} url URL。
 * @param {uint8_t} slave 从机地址。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t modbus_service_rtu_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               const char* url, uint8_t slave);

END_C_DECLS

#endif /*TK_MODBUS_SERVICE_RTU_H*/

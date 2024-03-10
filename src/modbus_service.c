/**
 * File:   modbus_service.c
 * Author: AWTK Develop Team
 * Brief:  modbus service
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
 * 2023-10-02 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "modbus_service.h"
#include "tkc/event_source_fd.h"

BEGIN_C_DECLS

ret_t modbus_service_destroy(modbus_service_t* service);
ret_t modbus_service_dispatch(modbus_service_t* service);

modbus_service_t* modbus_service_create_with_io(tk_iostream_t* io, modbus_proto_t proto,
                                                modbus_memory_t* memory) {
  modbus_service_t* service = NULL;

  return_value_if_fail(io != NULL, NULL);

  service = TKMEM_ZALLOC(modbus_service_t);
  return_value_if_fail(service != NULL, NULL);

  service->memory = memory;
  tk_service_init(&(service->service), io);
  modbus_common_init(MODBUS_COMMON(service), io, proto, &(service->service.wb));

  if (MODBUS_PROTO_TCP == proto) {
    modbus_service_set_slave(service, 0xff);
  }
  
  service->service.dispatch = (tk_service_dispatch_t)modbus_service_dispatch;
  service->service.destroy = (tk_service_destroy_t)modbus_service_destroy;
  service->service.io = io;


  return service;
}

ret_t modbus_service_dispatch(modbus_service_t* service) {
  ret_t ret = RET_OK;
  modbus_req_data_t req_data;
  modbus_resp_data_t resp_data;
  uint16_t buff[MODBUS_MAX_PDU_SIZE];
  modbus_exeption_code_t code = MODBUS_EXCEPTION_ILLEGAL_FUNCTION;
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  memset(buff, 0x00, sizeof(buff));
  memset(&req_data, 0x00, sizeof(req_data));
  memset(&resp_data, 0x00, sizeof(resp_data));

  ret = modbus_common_recv_req(MODBUS_COMMON(service), &req_data);

  if (ret == RET_EOS || ret == RET_IO) {
    return RET_REMOVE;
  }

  if (ret == RET_OK) {
    modbus_memory_t* memory = service->memory;

    if (req_data.slave != service->common.slave) {
#ifdef WITH_MULT_SLAVES
      log_debug("slave %d != %d, not send to me.\n", req_data.slave, service->common.slave);
#else
      log_debug("slave id not match: %d != %d\n", req_data.slave, service->common.slave);
      modbus_common_send_exception_resp(MODBUS_COMMON(service), req_data.func_code,
                                        MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
#endif
      return RET_OK;
    }

    resp_data.addr = req_data.addr;
    resp_data.count = req_data.count;
    resp_data.func_code = req_data.func_code;
    resp_data.data = (uint8_t*)buff;

    switch (req_data.func_code) {
      case MODBUS_FC_READ_COILS: {
        resp_data.bytes = (req_data.count + 7) / 8;
        ret = modbus_memory_read_bits(memory, req_data.addr, req_data.count, (uint8_t*)buff);
        break;
      }
      case MODBUS_FC_READ_DISCRETE_INPUTS: {
        resp_data.bytes = (req_data.count + 7) / 8;
        ret = modbus_memory_read_input_bits(memory, req_data.addr, req_data.count, (uint8_t*)buff);
        break;
      }
      case MODBUS_FC_READ_HOLDING_REGISTERS: {
        resp_data.bytes = req_data.count * 2;
        ret = modbus_memory_read_registers(memory, req_data.addr, req_data.count, buff);
        break;
      }
      case MODBUS_FC_READ_INPUT_REGISTERS: {
        resp_data.bytes = req_data.count * 2;
        ret = modbus_memory_read_input_registers(memory, req_data.addr, req_data.count, buff);
        break;
      }
      case MODBUS_FC_WRITE_SINGLE_COIL: {
        ret = modbus_memory_write_bit(memory, req_data.addr, req_data.data[0]);
        break;
      }
      case MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER: {
        ret = modbus_memory_write_register(memory, req_data.addr, *(uint16_t*)(req_data.data));
        break;
      }
      case MODBUS_FC_WRITE_MULTIPLE_COILS: {
        ret = modbus_memory_write_bits(memory, req_data.addr, req_data.count, req_data.data);
        break;
      }
      case MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS: {
        ret = modbus_memory_write_registers(memory, req_data.addr, req_data.count,
                                            (uint16_t*)req_data.data);
        break;
      }
      default: {
        ret = RET_NOT_IMPL;
        break;
      }
    }

    log_debug("%d done\n", req_data.func_code);

    if (ret == RET_OK) {
      return modbus_common_send_resp(MODBUS_COMMON(service), &resp_data);
    }
  }

  if (ret == RET_INVALID_ADDR) {
    code = MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS;
  } else if (ret == RET_CRC) {
    code = MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE;
  } else if (ret == RET_EXCEED_RANGE) {
    code = MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE;
  } else if (ret == RET_NOT_IMPL) {
    code = MODBUS_EXCEPTION_ILLEGAL_FUNCTION;
  }

  log_debug("%d failed\n", req_data.func_code);
  return modbus_common_send_exception_resp(MODBUS_COMMON(service), req_data.func_code, code);
}

static ret_t service_on_request(event_source_t* source) {
  event_source_fd_t* event_source_fd = (event_source_fd_t*)source;
  modbus_service_t* service = (modbus_service_t*)(event_source_fd->ctx);
  ret_t ret = modbus_service_dispatch(service);

  if (tk_object_get_prop_bool(TK_OBJECT(service->common.io), TK_STREAM_PROP_IS_OK, FALSE)) {
    ret = RET_OK;
  } else {
    ret = RET_REMOVE;
  }

  return ret;
}

ret_t modbus_service_attach_to_event_source_manager(modbus_service_t* service,
                                                    event_source_manager_t* esm) {
  return_value_if_fail(service != NULL && esm != NULL, RET_BAD_PARAMS);

  int fd = tk_object_get_prop_int(TK_OBJECT(service->common.io), TK_STREAM_PROP_FD, -1);
  event_source_t* source = event_source_fd_create(fd, service_on_request, service);
  event_source_manager_add(esm, source);
  OBJECT_UNREF(source);

  return RET_OK;
}

ret_t modbus_service_destroy(modbus_service_t* service) {
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  modbus_common_deinit(MODBUS_COMMON(service));
  TKMEM_FREE(service);

  return RET_OK;
}

ret_t modbus_service_wait_for_data(modbus_service_t* service, uint32_t timeout) {
  tk_iostream_t* io = NULL;
  tk_istream_t* in = NULL;
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  io = service->common.io;
  in = tk_iostream_get_istream(io);
  return_value_if_fail(in != NULL, RET_BAD_PARAMS);

  if (!tk_object_get_prop_bool(TK_OBJECT(in), TK_STREAM_PROP_IS_OK, FALSE)) {
    return RET_IO;
  }

  return tk_istream_wait_for_data(in, timeout);
}

ret_t modbus_service_run(modbus_service_t* service) {
  do {
    if (modbus_service_wait_for_data(service, 1000) == RET_OK) {
      if (modbus_service_dispatch(service) == RET_IO) break;
    } else {
      sleep_ms(10);
    }
  } while (TRUE);

  return RET_OK;
}


ret_t modbus_service_set_slave(modbus_service_t* service, uint8_t slave) {
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  service->common.slave = slave;

  return RET_OK;
}

tk_service_t* modbus_service_create(tk_iostream_t* io, void* args) {
  modbus_service_t* service = NULL;
  modbus_service_args_t* service_args = (modbus_service_args_t*)args;
  return_value_if_fail(io != NULL && args != NULL, NULL);

  service = modbus_service_create_with_io(io, service_args->proto, service_args->memory);
  return_value_if_fail(service != NULL, NULL);

  if (service_args->proto == MODBUS_PROTO_RTU) {
    modbus_service_set_slave(service, service_args->slave);
  }
  return (tk_service_t*)service;
}

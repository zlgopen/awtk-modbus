/**
 * File:   modbus_service_tcp.c
 * Author: AWTK Develop Team
 * Brief:  modbus service tcp
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

#include "modbus_service_tcp.h"
#include "streams/inet/iostream_tcp.h"

typedef struct _modbus_server_info_t {
  modbus_memory_t* memory;
  modbus_hook_t* hook;
  modbus_proto_t proto;
} modbus_server_info_t;

static modbus_server_info_t* modbus_server_info_create(modbus_memory_t* memory,
                                                       modbus_hook_t* hook, modbus_proto_t proto) {
  modbus_server_info_t* info = TKMEM_ZALLOC(modbus_server_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->memory = memory;
  info->hook = hook;
  info->proto = proto;

  return info;
}

static ret_t modbus_server_info_destroy(modbus_server_info_t* info) {
  if (info != NULL) {
    TKMEM_FREE(info);
  }

  return RET_OK;
}

static ret_t on_source_destroy(void* ctx, event_t* e) {
  modbus_server_info_destroy((modbus_server_info_t*)ctx);
  return RET_REMOVE;
}

static ret_t modbus_service_on_client(event_source_t* source) {
  event_source_fd_t* event_source_fd = (event_source_fd_t*)source;
  modbus_server_info_t* info = (modbus_server_info_t*)(event_source_fd->ctx);
  modbus_memory_t* memory = info->memory;
  modbus_hook_t* hook = info->hook;
  int listen_sock = event_source_get_fd(source);
  int sock = tcp_accept(listen_sock);

  if (sock >= 0) {
    log_debug("client connected:%d\n", sock);
    tk_iostream_t* io = tk_iostream_tcp_create(sock);
    if (io != NULL) {
      modbus_service_t* service = modbus_service_create_with_io(io, info->proto, memory);
      if (service != NULL) {
        modbus_service_attach_to_event_source_manager(service, source->manager);
        modbus_service_set_hook(service, hook);
      } else {
        TK_OBJECT_UNREF(io);
      }
    } else {
      log_debug("oom! disconnected:%d\n", sock);
      tk_socket_close(sock);
    }
  } else {
    log_debug("error disconnected:%d\n", sock);
    tk_socket_close(sock);
  }

  return RET_OK;
}

ret_t modbus_service_tcp_start(event_source_manager_t* esm, modbus_memory_t* memory,
                               modbus_hook_t* hook, int port, modbus_proto_t proto) {
  int sock = -1;
  int listen_sock = -1;
  event_source_t* source = NULL;
  return_value_if_fail(memory != NULL, RET_BAD_PARAMS);

  listen_sock = tcp_listen(port);
  log_debug("listen on %d listen_sock=%d\n", port, listen_sock);
  return_value_if_fail(listen_sock >= 0, RET_BAD_PARAMS);

  if (proto == MODBUS_PROTO_RTU) {
    log_debug("run rtu over tcp\n");
  }

  if (esm != NULL) {
    modbus_server_info_t* info = modbus_server_info_create(memory, hook, proto);
    return_value_if_fail(info != NULL, RET_OOM);
    source = event_source_fd_create(listen_sock, modbus_service_on_client, info);
    return_value_if_fail(source != NULL, RET_OOM);
    event_source_manager_add(esm, source);
    emitter_on(EMITTER(source), EVT_DESTROY, on_source_destroy, info);
    OBJECT_UNREF(source);

    return RET_OK;
  } else {
    tk_iostream_t* io = NULL;

    while (TRUE) {
      sock = tk_tcp_accept(listen_sock);
      if (sock <= 0) {
        break;
      }

      log_debug("net client connected:%d\n====================================\n", sock);
      io = tk_iostream_tcp_create(sock);
      if (io != NULL) {
        modbus_service_t* service = modbus_service_create_with_io(io, proto, memory);
        if (service != NULL) {
          modbus_service_set_hook(service, hook);
          modbus_service_run(service);
        }
        tk_socket_close(sock);
        modbus_service_destroy(service);
      } else {
        break;
      }
    }

    tk_socket_close(listen_sock);
    return RET_OK;
  }
}

#include "modbus_service.h"
#include "modbus_service_rtu.h"
#include "modbus_service_tcp.h"
#include "modbus_memory_default.h"
#include "modbus_memory_custom.h"
#include "tkc/socket_helper.h"
#include "streams/stream_factory.h"
#include "streams/inet/iostream_tcp.h"

#include "service/service.h"
#include "tkc/thread.h"
#include "streams/serial/iostream_serial.h"

static bool_t s_modbus_running = FALSE;
static tk_thread_t* s_modbus_thread = NULL;

static void* modbus_service_func(void* args) {
  const char* device = (const char*)args;
  tk_iostream_t* io = tk_iostream_serial_create(device);
  modbus_memory_t* memory = modbus_memory_custom_create();
  modbus_service_t* service = modbus_service_create_with_io(io, MODBUS_PROTO_RTU, memory);

  modbus_service_set_slave(service, MODBUS_DEMO_SLAVE_ID);
  while (s_modbus_running) {
    modbus_service_run(service);
    sleep_ms(50);
  }

  modbus_memory_destroy(memory);
  modbus_service_destroy(service);

  return 0;
}

ret_t modbus_service_start(const char* device) {
  return_value_if_fail(s_modbus_running == FALSE, RET_OK);
  return_value_if_equal(s_modbus_thread == NULL, RET_FAIL);

  s_modbus_thread = tk_thread_create(modbus_service_func, (void*)device);
  return_value_if_fail(s_modbus_thread != NULL, RET_FAIL);

  tk_thread_set_name(s_modbus_thread, "modbus");
  tk_thread_set_stack_size(s_modbus_thread, 0x2000);
	
  tk_thread_start(s_modbus_thread);
  s_modbus_running = TRUE;

  return RET_OK;
}

ret_t modbus_service_stop(void) {
  return_value_if_fail(s_modbus_running == TRUE, RET_OK);

  s_modbus_running = FALSE;
  tk_thread_destroy(s_modbus_thread);
  s_modbus_thread = NULL;

  return RET_OK;
}


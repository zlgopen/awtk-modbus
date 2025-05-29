#include "modbus_service_tcp.h"

static bool_t started = FALSE;
static bool_t running = FALSE;

static modbus_memory_t* modbus_memory_default_create_foo(void) {
  uint32_t i = 0;
  uint32_t n = 10000;
  uint16_t* p = NULL;
  modbus_server_channel_t* bits =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_BITS, 0, n, TRUE);
  modbus_server_channel_t* input_bits =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_INPUT_BITS, 0, n, FALSE);

  p = (uint16_t*)input_bits->data;
  for (i = 0; i < input_bits->bytes/2; i++) {
    p[i] = i;
  }

  modbus_server_channel_t* registers =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_REGISTERS, 0, n, TRUE);

  modbus_server_channel_t* input_registers =
      modbus_server_channel_create(MODBUS_SERVER_CHANNEL_INPUT_REGISTERS, 0, n, FALSE);
  
  p = (uint16_t*)input_registers->data;
  for (i = 0; i < n; i++) {
    p[i] = i * 2;
  }


  return modbus_memory_default_create(bits, input_bits, registers, input_registers);
}

static void* thread_server_func(void* ctx) {
  int port = 2502;
  event_source_manager_t* esm = NULL;
  modbus_service_args_t* args = (modbus_service_args_t*)ctx;

  tk_socket_init();

  esm = event_source_manager_default_create();
  modbus_service_tcp_start_by_args(esm, args, port);

  started = TRUE;
  while (running) {
    event_source_manager_dispatch(esm);
  }

  event_source_manager_destroy(esm);
  tk_socket_deinit();

  log_info("thread_server_func exit");
  return NULL;
}

static int s_connected = 0;
static int s_ended = 0;
static modbus_service_t* s_service = NULL;

static ret_t modbus_service_on_disconnected(modbus_service_t* service, void* ctx) {
  s_ended++;
  return RET_OK;
}

static ret_t modbus_service_on_connected(modbus_service_t* service, void* ctx) {
  s_connected++;
  s_service = service;
  service->on_disconnected = modbus_service_on_disconnected;
  return RET_OK;
}

static tk_thread_t* create_modbus_service(uint8_t slave, modbus_memory_t* memory) {
  static modbus_service_args_t args;
  memset(&args, 0x0, sizeof(modbus_service_args_t));
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = memory;
  args.on_connected = modbus_service_on_connected;
  s_connected = 0;
  s_ended = 0;

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  started = FALSE;
  tk_thread_start(thread);
 
  while (!started) {
    sleep_ms(100);
  }

  return thread;
}

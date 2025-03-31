#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "conf_io/conf_json.h"
#include "modbus_client_channel.h"
#include "modbus_service.h"
#include "modbus_memory_default.h"

static modbus_client_channel_t* modbus_client_channel_create_with_json(const char* filename) {
  tk_object_t* obj = conf_json_load(filename, FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  TK_OBJECT_UNREF(obj);

  return channel; 
}

TEST(modbus_client_channel, read_bits) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/read_bits.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 1);
  ASSERT_STREQ(channel->name, "read_bits");
  ASSERT_EQ(channel->access_type, 1);
  ASSERT_EQ(channel->read_offset, 0x130);
  ASSERT_EQ(channel->bits_length, 0x10);
  ASSERT_EQ(channel->read_buffer_length, 2);
  ASSERT_EQ(channel->write_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer != NULL, true);
  ASSERT_EQ(channel->read_buffer != NULL, true);
  ASSERT_EQ(channel->write_buffer == NULL, true);
  ASSERT_EQ(channel->keep_last_value_if_read_failed, TRUE);

  modbus_client_channel_destroy(channel);
  TK_OBJECT_UNREF(obj);
}

TEST(modbus_client_channel, read_input_bits) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/read_input_bits.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 1);
  ASSERT_STREQ(channel->name, "read_input_bits");
  ASSERT_EQ(channel->access_type, 2);
  ASSERT_EQ(channel->read_offset, 0x130);
  ASSERT_EQ(channel->bits_length, 0x10);
  ASSERT_EQ(channel->read_buffer_length, 2);
  ASSERT_EQ(channel->write_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer != NULL, true);
  ASSERT_EQ(channel->read_buffer != NULL, true);
  ASSERT_EQ(channel->write_buffer == NULL, true);
  ASSERT_EQ(channel->keep_last_value_if_read_failed, TRUE);

  modbus_client_channel_destroy(channel);
  TK_OBJECT_UNREF(obj);
}

TEST(modbus_client_channel, read_registers) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/read_registers.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 2);
  ASSERT_STREQ(channel->name, "read_registers");
  ASSERT_EQ(channel->access_type, 3);
  ASSERT_EQ(channel->read_offset, 0x10);
  ASSERT_EQ(channel->bits_length, 0);
  ASSERT_EQ(channel->read_buffer_length, 16);
  ASSERT_EQ(channel->write_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer == NULL, true);
  ASSERT_EQ(channel->read_buffer != NULL, true);
  ASSERT_EQ(channel->write_buffer == NULL, true);
  ASSERT_EQ(channel->keep_last_value_if_read_failed, FALSE);

  modbus_client_channel_destroy(channel);
  TK_OBJECT_UNREF(obj);
}

TEST(modbus_client_channel, read_input_registers) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/read_input_registers.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 2);
  ASSERT_STREQ(channel->name, "read_input_registers");
  ASSERT_EQ(channel->access_type, 4);
  ASSERT_EQ(channel->read_offset, 0x10);
  ASSERT_EQ(channel->bits_length, 0);
  ASSERT_EQ(channel->read_buffer_length, 16);
  ASSERT_EQ(channel->write_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer == NULL, true);
  ASSERT_EQ(channel->read_buffer != NULL, true);
  ASSERT_EQ(channel->write_buffer == NULL, true);
  ASSERT_EQ(channel->keep_last_value_if_read_failed, FALSE);

  modbus_client_channel_destroy(channel);
  TK_OBJECT_UNREF(obj);
}

TEST(modbus_client_channel, write_1_bit) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/write_1_bit.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 1);
  ASSERT_STREQ(channel->name, "write_1_bit");
  ASSERT_EQ(channel->access_type, 5);
  ASSERT_EQ(channel->write_offset, 0x130);
  ASSERT_EQ(channel->bits_length, 1);
  ASSERT_EQ(channel->write_buffer_length, 1);
  ASSERT_EQ(channel->read_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer != NULL, true);
  ASSERT_EQ(channel->write_buffer != NULL, true);
  ASSERT_EQ(channel->read_buffer == NULL, true);

  TK_OBJECT_UNREF(obj);
  modbus_client_channel_destroy(channel);
}

TEST(modbus_client_channel, write_bits) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/write_bits.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 1);
  ASSERT_STREQ(channel->name, "write_bits");
  ASSERT_EQ(channel->access_type, 15);
  ASSERT_EQ(channel->write_offset, 0x130);
  ASSERT_EQ(channel->bits_length, 16);
  ASSERT_EQ(channel->write_buffer_length, 2);
  ASSERT_EQ(channel->read_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer != NULL, true);
  ASSERT_EQ(channel->write_buffer != NULL, true);
  ASSERT_EQ(channel->read_buffer == NULL, true);

  TK_OBJECT_UNREF(obj);
  modbus_client_channel_destroy(channel);
}

TEST(modbus_client_channel, write_1_register) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/write_1_register.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);

  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 1);
  ASSERT_STREQ(channel->name, "write_1_register");
  ASSERT_EQ(channel->access_type, 6);
  ASSERT_EQ(channel->write_offset, 0x130);
  ASSERT_EQ(channel->bits_length, 0);
  ASSERT_EQ(channel->write_buffer_length, 2);
  ASSERT_EQ(channel->read_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer == NULL, true);
  ASSERT_EQ(channel->write_buffer != NULL, true);
  ASSERT_EQ(channel->read_buffer == NULL, true);

  TK_OBJECT_UNREF(obj);
  modbus_client_channel_destroy(channel);
}

TEST(modbus_client_channel, write_registers) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/write_registers.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);
  return_if_fail(conf != NULL);
  modbus_client_channel_t* channel = modbus_client_channel_create(conf->root);
  ASSERT_EQ(channel->unit_id, 1);
  ASSERT_STREQ(channel->name, "write_registers");
  ASSERT_EQ(channel->access_type, 16);
  ASSERT_EQ(channel->write_offset, 0x130);
  ASSERT_EQ(channel->bits_length, 0);
  ASSERT_EQ(channel->write_buffer_length, 32);
  ASSERT_EQ(channel->read_buffer_length, 0);

  ASSERT_EQ(channel->bits_buffer == NULL, true);
  ASSERT_EQ(channel->write_buffer != NULL, true);
  ASSERT_EQ(channel->read_buffer == NULL, true);

  TK_OBJECT_UNREF(obj);
  modbus_client_channel_destroy(channel);
}

#include "tkc/socket_helper.h"
#include "streams/stream_factory.h"
#include "streams/inet/iostream_tcp.h"
#include "modbus_service_tcp.h"
#include "modbus_client.h"

static bool_t running = TRUE;

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

  while (running) {
    event_source_manager_dispatch(esm);
    sleep_ms(50);
  }

  event_source_manager_destroy(esm);
  tk_socket_deinit();

  log_info("thread_server_func exit");
  return NULL;
}

TEST(modbus_client_channel, with_server_bits) {
  uint32_t i = 0;
  uint16_t* w = NULL;
  uint16_t* r = NULL;
  uint8_t slave = 0xFF;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
  modbus_client_channel_t* write_bits =
      modbus_client_channel_create_with_json("file://./tests/testdata/write_bits_6000.json");
  modbus_client_channel_t* read_bits =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_bits_6000.json");

  w = (uint16_t*)write_bits->write_buffer;
  for (i = 0; i < write_bits->write_buffer_length / 2; i++) {
    w[i] = i;
  }
  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(write_bits, client);
  modbus_client_channel_set_client(read_bits, client);

  ASSERT_EQ(modbus_client_channel_write(write_bits), RET_OK);
  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_OK);

  r = (uint16_t*)read_bits->read_buffer;
  for (i = 0; i < read_bits->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(write_bits);
  modbus_client_channel_destroy(read_bits);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_registers) {
  uint32_t i = 0;
  uint16_t* w = NULL;
  uint16_t* r = NULL;
  uint8_t slave = 1;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
  modbus_client_channel_t* write_registers =
      modbus_client_channel_create_with_json("file://./tests/testdata/write_registers_1000.json");
  modbus_client_channel_t* read_registers =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_registers_1000.json");

  w = (uint16_t*)write_registers->write_buffer;
  for (i = 0; i < write_registers->write_buffer_length / 2; i++) {
    w[i] = i;
  }

  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(write_registers, client);
  modbus_client_channel_set_client(read_registers, client);

  ASSERT_EQ(modbus_client_channel_write(write_registers), RET_OK);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_OK);

  r = (uint16_t*)read_registers->read_buffer;
  for (i = 0; i < read_registers->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(write_registers);
  modbus_client_channel_destroy(read_registers);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_input_bits) {
  uint32_t i = 0;
  uint16_t* r = NULL;
  uint8_t slave = 1;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
      modbus_client_channel_create_with_json("file://./tests/testdata/write_bits_6000.json");
  modbus_client_channel_t* read_bits =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_input_bits_6000.json");

  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(read_bits, client);

  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_OK);

  r = (uint16_t*)read_bits->read_buffer;
  for (i = 0; i < read_bits->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(read_bits);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_input_registers) {
  uint32_t i = 0;
  uint16_t* r = NULL;
  uint8_t slave = 2;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
  modbus_client_channel_t* read_registers =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_input_registers_1000.json");

  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(read_registers, client);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_OK);

  r = (uint16_t*)read_registers->read_buffer;
  for (i = 0; i < read_registers->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i * 2);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(read_registers);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_bits_auto_reconnect) {
  uint32_t i = 0;
  uint16_t* w = NULL;
  uint16_t* r = NULL;
  uint8_t slave = 0xFF;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
  modbus_client_channel_t* write_bits =
      modbus_client_channel_create_with_json("file://./tests/testdata/write_bits_6000.json");
  modbus_client_channel_t* read_bits =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_bits_6000.json");

  w = (uint16_t*)write_bits->write_buffer;
  for (i = 0; i < write_bits->write_buffer_length / 2; i++) {
    w[i] = i;
  }
  modbus_client_set_auto_reconnect(client, TRUE);
  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(write_bits, client);
  modbus_client_channel_set_client(read_bits, client);

  ASSERT_EQ(modbus_client_channel_write(write_bits), RET_OK);
  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_OK);

  r = (uint16_t*)read_bits->read_buffer;
  for (i = 0; i < read_bits->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);

  ASSERT_EQ(modbus_client_channel_write(write_bits), RET_IO);
  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_IO);

  thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);

  memset(read_bits->read_buffer, 0x0, read_bits->read_buffer_length);
  ASSERT_EQ(modbus_client_channel_write(write_bits), RET_OK);
  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_OK);

  r = (uint16_t*)read_bits->read_buffer;
  for (i = 0; i < read_bits->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(write_bits);
  modbus_client_channel_destroy(read_bits);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_registers_auto_reconnect) {
  uint32_t i = 0;
  uint16_t* w = NULL;
  uint16_t* r = NULL;
  uint8_t slave = 1;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
  modbus_client_channel_t* write_registers =
      modbus_client_channel_create_with_json("file://./tests/testdata/write_registers_1000.json");
  modbus_client_channel_t* read_registers =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_registers_1000.json");

  w = (uint16_t*)write_registers->write_buffer;
  for (i = 0; i < write_registers->write_buffer_length / 2; i++) {
    w[i] = i;
  }

  modbus_client_set_auto_reconnect(client, TRUE);
  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(write_registers, client);
  modbus_client_channel_set_client(read_registers, client);

  ASSERT_EQ(modbus_client_channel_write(write_registers), RET_OK);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_OK);

  r = (uint16_t*)read_registers->read_buffer;
  for (i = 0; i < read_registers->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);

  ASSERT_EQ(modbus_client_channel_write(write_registers), RET_IO);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_IO);

  thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);

  memset(read_registers->read_buffer, 0x0, read_registers->read_buffer_length); 
  ASSERT_EQ(modbus_client_channel_write(write_registers), RET_OK);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_OK);

  r = (uint16_t*)read_registers->read_buffer;
  for (i = 0; i < read_registers->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(write_registers);
  modbus_client_channel_destroy(read_registers);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_input_bits_auto_reconnect) {
  uint32_t i = 0;
  uint16_t* r = NULL;
  uint8_t slave = 1;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
      modbus_client_channel_create_with_json("file://./tests/testdata/write_bits_6000.json");
  modbus_client_channel_t* read_bits =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_input_bits_6000.json");

  modbus_client_set_auto_reconnect(client, TRUE);
  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(read_bits, client);

  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_OK);

  r = (uint16_t*)read_bits->read_buffer;
  for (i = 0; i < read_bits->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  
  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_IO);
  thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);

  memset(read_bits->read_buffer, 0x0, read_bits->read_buffer_length);
  ASSERT_EQ(modbus_client_channel_read(read_bits), RET_OK);
  r = (uint16_t*)read_bits->read_buffer;
  for (i = 0; i < read_bits->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(read_bits);
  modbus_memory_destroy(args.memory);
}

TEST(modbus_client_channel, with_server_input_registers_auto_reconnect) {
  uint32_t i = 0;
  uint16_t* r = NULL;
  uint8_t slave = 2;
  modbus_service_args_t args;
  args.slave = slave;
  args.proto = MODBUS_PROTO_TCP;
  args.memory = modbus_memory_default_create_foo();

  tk_thread_t* thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");
  modbus_client_channel_t* read_registers =
      modbus_client_channel_create_with_json("file://./tests/testdata/read_input_registers_1000.json");

  modbus_client_set_auto_reconnect(client, TRUE);
  modbus_client_set_slave(client, slave);
  modbus_client_channel_set_client(read_registers, client);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_OK);

  r = (uint16_t*)read_registers->read_buffer;
  for (i = 0; i < read_registers->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i * 2);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);

  memset(read_registers->read_buffer, 0x0, read_registers->read_buffer_length);
  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_IO);
  
  thread = tk_thread_create(thread_server_func, &args);
  running = TRUE;
  tk_thread_start(thread);
  sleep_ms(1000);

  ASSERT_EQ(modbus_client_channel_read(read_registers), RET_OK);

  r = (uint16_t*)read_registers->read_buffer;
  for (i = 0; i < read_registers->read_buffer_length / 2; i++) {
    ASSERT_EQ(r[i], i * 2);
  }

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_client_channel_destroy(read_registers);
  modbus_memory_destroy(args.memory);
}

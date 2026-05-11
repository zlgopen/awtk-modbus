#include "gtest/gtest.h"
#include <thread>
#include "tkc/utils.h"
#include "conf_io/conf_json.h"
#include "modbus_client_channel.h"
#include "modbus_service.h"
#include "modbus_memory_default.h"

#include "modbus_service_helper.h"

TEST(modbus_client, write_registers) {
  modbus_memory_t* memory = modbus_memory_default_create_foo();
  modbus_memory_default_t* default_memory = (modbus_memory_default_t*)memory;
  tk_thread_t* thread = create_modbus_service(0xff, memory);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");

  uint16_t* data = (uint16_t*)(default_memory->registers->data);
  for (uint32_t i = 0; i < 100; i++) {
    uint16_t write_registers[] = {0x1122, 0x2233, 0x3344, 0x4455};
    uint16_t addr = i * 4;
    ret_t ret =  modbus_client_write_registers(client, addr, 4, write_registers);
    ASSERT_EQ(ret, RET_OK);
    ASSERT_EQ(data[0], write_registers[0]);
    ASSERT_EQ(data[1], write_registers[1]);
    ASSERT_EQ(data[2], write_registers[2]);
    ASSERT_EQ(data[3], write_registers[3]);
    data += 4;
  }
  
  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_memory_destroy(memory);
  modbus_client_destroy(client);
}

TEST(modbus_client, write_register) {
  modbus_memory_t* memory = modbus_memory_default_create_foo();
  modbus_memory_default_t* default_memory = (modbus_memory_default_t*)memory;
  tk_thread_t* thread = create_modbus_service(0xff, memory);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");

  uint16_t* data = (uint16_t*)(default_memory->registers->data);
  for (uint32_t i = 0; i < 1000; i++) {
    uint16_t addr = i;
    ret_t ret =  modbus_client_write_register(client, addr, i);
    ASSERT_EQ(ret, RET_OK);
    ASSERT_EQ(data[i], i);
  }
  
  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_memory_destroy(memory);
  modbus_client_destroy(client);
}

static void test_modbus_client_all(modbus_client_t* client, modbus_memory_default_t* default_memory) {
  uint8_t addr = 0x01;
  bool_t value;

  uint8_t read_bits_result[4];
  bool_t write_bit = TRUE;
  uint8_t write_bits[] = {TRUE, FALSE, TRUE, FALSE};

  uint16_t read_registers_result[4];
  uint16_t write_register = 0xaabb;
  uint16_t write_registers[] = {0x1122, 0x2233, 0x3344, 0x4455};

  uint8_t* bits_data = default_memory->bits->data;
  uint16_t* registers_data = (uint16_t*)(default_memory->registers->data);
  // 为0为了方便后续直接使用addr访问地址
  ASSERT_EQ(default_memory->bits->start, 0);
  ASSERT_EQ(default_memory->registers->start, 0);

  // === bit ===
  ASSERT_EQ(modbus_client_write_bit(client, addr, write_bit), RET_OK);
  ASSERT_EQ(bits_stream_get(bits_data, default_memory->bits->bytes, addr, &value), RET_OK);
  ASSERT_EQ(value, write_bit);
  ASSERT_EQ(modbus_client_read_bits(client, addr, 1, read_bits_result), RET_OK);
  ASSERT_EQ(read_bits_result[0], write_bit);

  // === bits ===
  ASSERT_EQ(modbus_client_write_bits(client, addr, 4, write_bits), RET_OK);
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(bits_stream_get(bits_data, default_memory->bits->bytes, addr + i, &value), RET_OK);
    ASSERT_EQ(value, write_bits[i]);
  }
  ASSERT_EQ(modbus_client_read_bits(client, addr, 4, read_bits_result), RET_OK);
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(read_bits_result[i], write_bits[i]);
  }

  // === register ===
  ASSERT_EQ(modbus_client_write_register(client, addr, write_register), RET_OK);
  ASSERT_EQ(registers_data[addr], write_register);
  ASSERT_EQ(modbus_client_read_registers(client, addr, 1, read_registers_result), RET_OK);
  ASSERT_EQ(read_registers_result[0], write_register);

  // === registers ===
  ASSERT_EQ(modbus_client_write_registers(client, addr, 4, write_registers), RET_OK);
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(registers_data[addr + i], write_registers[i]);
  }
  ASSERT_EQ(modbus_client_read_registers(client, addr, 4, read_registers_result), RET_OK);
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(read_registers_result[i], write_registers[i]);
  }

  memset(registers_data, 0x0, default_memory->registers->bytes);
  ASSERT_EQ(modbus_client_write_and_read_registers(client, addr, 4, write_registers, addr, 4, read_registers_result), RET_OK);
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(registers_data[addr + i], write_registers[i]); // write
    ASSERT_EQ(read_registers_result[i], write_registers[i]); // read
  }

  // === input_xx ===
  uint8_t* input_bits_data = default_memory->input_bits->data;
  uint16_t* input_registers_data = (uint16_t*)(default_memory->input_registers->data);

  // init input_xx data
  addr = 0x00;
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(bits_stream_set(input_bits_data, default_memory->input_bits->bytes, i, write_bits[i]), RET_OK);
    input_registers_data[i] = write_registers[i];
  }

  ASSERT_EQ(modbus_client_read_input_bits(client, addr, 4, read_bits_result), RET_OK);
  ASSERT_EQ(modbus_client_read_input_registers(client, addr, 4, read_registers_result), RET_OK);
  for (int i = 0; i < 4; ++i) {
    ASSERT_EQ(read_bits_result[i], write_bits[i]);
    ASSERT_EQ(read_registers_result[i], write_registers[i]);
  }
}

TEST(modbus_client, tcp_all) {
  modbus_memory_t* memory = modbus_memory_default_create_foo();
  modbus_memory_default_t* default_memory = (modbus_memory_default_t*)memory;
  tk_thread_t* thread = create_modbus_service(0xff, memory);
  sleep_ms(1000);
  modbus_client_t* client = modbus_client_create("tcp://localhost:2502");

  ASSERT_NO_FATAL_FAILURE(test_modbus_client_all(client, default_memory));

  running = FALSE;
  tk_thread_destroy(thread);
  sleep_ms(1000);
  modbus_memory_destroy(memory);
  modbus_client_destroy(client);
}

TEST(modbus_client, rtu_over_tcp_all) {
  modbus_memory_t* memory = modbus_memory_default_create_foo();
  modbus_memory_default_t* default_memory = (modbus_memory_default_t*)memory;
  event_source_manager_t* esm = event_source_manager_default_create();

  modbus_service_args_t args = {};
  args.memory = memory;
  args.slave = 0x01;
  args.proto = MODBUS_PROTO_RTU;

  ASSERT_EQ(modbus_service_tcp_start_by_args(esm, &args, 2502), RET_OK);
  bool running = true;
  std::thread thread = std::thread([esm, &running]() {
    while (running) {
      event_source_manager_dispatch(esm);
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
  });
  modbus_client_t* client = modbus_client_create("rtu+tcp://localhost:2502");
  modbus_client_set_slave(client, args.slave);

  ASSERT_NO_FATAL_FAILURE(test_modbus_client_all(client, default_memory));

  running = false;
  if (thread.joinable()) {
    thread.join();
  }
  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
  modbus_client_destroy(client);
}

#if 0
// 需要设置两个虚拟串口设备才能测试
#include "modbus_service_rtu.h"
#define VIRTUAL_SERVER_COM "COM4"
#define VIRTUAL_CLIENT_COM "COM5"

TEST(modbus_client, rtu_all) {
  const char server_url[] = "serial://" VIRTUAL_SERVER_COM "?baudrate=9600&stopbits=1&parity=none&flowcontrol=none&bytesize=8";
  const char client_url[] = "serial://" VIRTUAL_CLIENT_COM "?baudrate=9600&stopbits=1&parity=none&flowcontrol=none&bytesize=8";

  modbus_memory_t* memory = modbus_memory_default_create_foo();
  modbus_memory_default_t* default_memory = (modbus_memory_default_t*)memory;
  event_source_manager_t* esm = event_source_manager_default_create();

  modbus_service_args_t args = {};
  args.memory = memory;
  args.slave = 0x01;
  args.proto = MODBUS_PROTO_RTU;

  ASSERT_EQ(modbus_service_rtu_start_by_args(esm, &args, server_url), RET_OK);
  bool running = true;
  std::thread thread = std::thread([esm, &running]() {
    while (running) {
      event_source_manager_dispatch(esm);
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
  });
  modbus_client_t* client = modbus_client_create(client_url);
  modbus_client_set_slave(client, args.slave);

  ASSERT_NO_FATAL_FAILURE(test_modbus_client_all(client, default_memory));

  running = false;
  if (thread.joinable()) {
    thread.join();
  }
  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
  modbus_client_destroy(client);
}
#endif
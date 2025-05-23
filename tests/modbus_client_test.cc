#include "gtest/gtest.h"
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


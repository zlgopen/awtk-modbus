#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "awtk.h"
#include "modbus_service_tcp.h"
#include "modbus_service_rtu.h"
#include "modbus_memory_default.h"

static ret_t modbus_service_start(event_source_manager_t* esm, modbus_memory_t* memory, const char* url) {
  ret_t ret = RET_FAIL;
  if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) || tk_str_start_with(url, STR_SCHEMA_TCP)) {
    const char* p = strrchr(url, ':');
    int port = p != NULL ? tk_atoi(p + 1) : 502;
    modbus_proto_t proto =
        tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
    ret = modbus_service_tcp_start(esm, memory, port, proto, MODBUS_DEMO_SLAVE_ID);
  } else {
    ret = modbus_service_rtu_start(esm, memory, url, MODBUS_DEMO_SLAVE_ID);
  }
  return ret;
}

static ret_t modbus_service_start_by_args(event_source_manager_t* esm, modbus_service_args_t* args, const char* url) {
  ret_t ret = RET_FAIL;
  if (tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) || tk_str_start_with(url, STR_SCHEMA_TCP)) {
    const char* p = strrchr(url, ':');
    int port = p != NULL ? tk_atoi(p + 1) : 502;
    ret = modbus_service_tcp_start_by_args(esm, args, port);
  } else {
    ret = modbus_service_rtu_start_by_args(esm, args, url);
  }
  return ret;
}

TEST(modbus, server_tcp_init) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  event_source_manager_t* esm = event_source_manager_default_create();

  socket_init();

  const char* url = "tcp://localhost:502";
  ASSERT_EQ(modbus_service_start(esm, memory, url), RET_OK);
#ifdef NDEBUG
  url = "tcp://localhost:503";
  ASSERT_EQ(modbus_service_start(esm, memory, url), RET_FAIL);
#endif

  ASSERT_EQ(esm->sources.size, 1);
  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
}

TEST(modbus, server_tcp_init_by_args) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  event_source_manager_t* esm = event_source_manager_default_create();

  socket_init();

  const char* url = "tcp://localhost:502";
  modbus_service_args_t args1;
  args1.memory = memory;
  args1.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;;
  args1.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args1, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 1);

  url = "tcp://localhost:503";
  modbus_service_args_t args2;
  args2.memory = memory;
  args2.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;;
  args2.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args2, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 2);

  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
}

#if 0
// 需要有串口设备才可以测试
TEST(modbus, server_rtu_init) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  event_source_manager_t* esm = event_source_manager_default_create();

  socket_init();

  const char* url = "serial://COM1?baudrate=115200";
  ASSERT_EQ(modbus_service_start(esm, memory, url), RET_OK);
#ifdef NDEBUG
  url = "serial://COM1?baudrate=115200";
  ASSERT_EQ(modbus_service_start(esm, memory, url), RET_FAIL);
#endif

  ASSERT_EQ(esm->sources.size, 1);
  event_source_manager_destroy(esm);
}

TEST(modbus, server_rtu_init_by_args) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  event_source_manager_t* esm = event_source_manager_default_create();

  socket_init();

  const char* url = "serial://COM9?baudrate=115200";
  modbus_service_args_t args1;
  args1.memory = memory;
  args1.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;;
  args1.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args1, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 1);

  url = "serial://COM7?baudrate=115200";
  modbus_service_args_t args2;
  args2.memory = memory;
  args2.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;;
  args2.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args2, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 2);

  event_source_manager_destroy(esm);
}

#endif

#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "awtk.h"
#include "modbus_service_tcp.h"
#include "modbus_service_rtu.h"
#include "modbus_memory_default.h"
#include "modbus_service_helper.h"
#include "modbus_client.h"
#include <thread>

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

  tk_socket_init();

  const char* url = "tcp://localhost:502";
  ASSERT_EQ(modbus_service_tcp_is_started(), FALSE);
  ASSERT_EQ(modbus_service_start(esm, memory, url), RET_OK);
  ASSERT_EQ(modbus_service_tcp_is_started(), TRUE);
  url = "tcp://localhost:503";
  ASSERT_EQ(modbus_service_start(esm, memory, url), RET_FAIL);

  ASSERT_EQ(esm->sources.size, 1);
  ASSERT_EQ(modbus_service_tcp_stop(), RET_OK);
  ASSERT_EQ(modbus_service_tcp_is_started(), FALSE);
  ASSERT_EQ(modbus_service_tcp_stop(), RET_OK);
  ASSERT_EQ(esm->sources.size, 0);
  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
}

TEST(modbus, server_tcp_init_by_args) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  event_source_manager_t* esm = event_source_manager_default_create();

  tk_socket_init();

  const char* url = "tcp://localhost:502";
  ASSERT_EQ(modbus_service_tcp_is_started(), FALSE);
  modbus_service_args_t args1 = {};
  args1.memory = memory;
  args1.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
  args1.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args1, url), RET_OK);
  ASSERT_EQ(modbus_service_tcp_is_started(), TRUE);
  ASSERT_EQ(esm->sources.size, 1);

  url = "tcp://localhost:503";
  modbus_service_args_t args2 = {};
  args2.memory = memory;
  args2.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
  args2.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args2, url), RET_FAIL);
  ASSERT_EQ(esm->sources.size, 1);
#if 0
  // 指定网卡启动 modbus 服务
  url = "tcp://localhost:503";
  modbus_service_args_t args3 = {};
  args3.memory = memory;
  args3.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
  args3.slave = MODBUS_DEMO_SLAVE_ID;
  args3.ifname = L"以太网 3"; //指定网卡名字
  ASSERT_EQ(modbus_service_start_by_args(esm, &args3, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 3);
#endif

  ASSERT_EQ(modbus_service_tcp_stop(), RET_OK);
  ASSERT_EQ(modbus_service_tcp_is_started(), FALSE);
  ASSERT_EQ(modbus_service_tcp_stop(), RET_OK);
  ASSERT_EQ(esm->sources.size, 0);
  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
}

TEST(modbus, server_rtu_stop_without_start) {
  ASSERT_EQ(modbus_service_rtu_is_started(), FALSE);
  ASSERT_EQ(modbus_service_rtu_stop(), RET_OK);
  ASSERT_EQ(modbus_service_rtu_is_started(), FALSE);
}

static void test_server_slave_error(const char* server_url, const char* client_url, uint8_t slave, modbus_proto_t proto) {
  modbus_memory_t* memory = modbus_memory_default_create_foo();
  event_source_manager_t* esm = event_source_manager_default_create();

  modbus_service_args_t args = {};
  args.memory = memory;
  args.slave = slave;
  args.proto = proto;

  if (tk_str_start_with(server_url, STR_SCHEMA_SERIAL)) {
    ASSERT_EQ(modbus_service_rtu_start_by_args(esm, &args, server_url), RET_OK);
  } else {
    const char* p = strrchr(server_url, ':');
    int port = p != NULL ? tk_atoi(p + 1) : 502;
    ASSERT_EQ(modbus_service_tcp_start_by_args(esm, &args, port), RET_OK); // tcp://localhost:xxxx
  }

  bool running = true;
  std::thread thread = std::thread([esm, &running]() {
    while (running) {
      event_source_manager_dispatch(esm);
      std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
  });

  // client
  modbus_client_t* client = modbus_client_create(client_url);
  uint16_t buff[2] = {0};
  // RTU: 02 10 00 00 00 02 04 00 00 00 00 CRC
  const int test_times = 10;
  int times_arr1[test_times] = {1, 2, 0, 0, 0, 1, 2, 3, 1, 3};
  int times_arr2[test_times] = {1, 1, 2, 3, 1, 0, 1, 2, 2, 3};
  for(int i = 0; i < test_times; ++i) {
    if(slave == i) continue;

    // 错误的从站地址
    for(int j = 0; j < times_arr1[i]; ++j) {
      modbus_client_set_slave(client, i);
      ASSERT_EQ(modbus_client_write_registers(client, 0, 2, buff) != RET_OK, true);
    }

    // 正常的写操作
    for(int j = 0; j < times_arr2[i]; ++j) {
      modbus_client_set_slave(client, slave);
      ASSERT_EQ(modbus_client_write_registers(client, 0, 2, buff), RET_OK);
    }
  }

  running = false;
  if (thread.joinable()) {
    thread.join();
  }
  event_source_manager_destroy(esm);
  modbus_memory_destroy(memory);
  modbus_client_destroy(client);
}

TEST(modbus, server_rtu_over_tcp_error) {
  test_server_slave_error("rtu+tcp://localhost:502", "rtu+tcp://localhost:502", 0x01, MODBUS_PROTO_RTU);
}

TEST(modbus, server_tcp_error) {
  test_server_slave_error("tcp://localhost:502", "tcp://localhost:502", 0xff, MODBUS_PROTO_TCP);
}

#if 0
// 需要有串口设备才可以测试
TEST(modbus, server_rtu_init) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  event_source_manager_t* esm = event_source_manager_default_create();

  tk_socket_init();

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

  tk_socket_init();

  const char* url = "serial://COM9?baudrate=115200";
  modbus_service_args_t args1 = {};
  args1.memory = memory;
  args1.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
  args1.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args1, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 1);

  url = "serial://COM7?baudrate=115200";
  modbus_service_args_t args2 = {};
  args2.memory = memory;
  args2.proto = tk_str_start_with(url, STR_SCHEMA_RTU_OVER_TCP) ? MODBUS_PROTO_RTU : MODBUS_PROTO_TCP;
  args2.slave = MODBUS_DEMO_SLAVE_ID;
  ASSERT_EQ(modbus_service_start_by_args(esm, &args2, url), RET_OK);
  ASSERT_EQ(esm->sources.size, 2);

  event_source_manager_destroy(esm);
}

#endif

#if 0
// 需要设置两个虚拟串口设备才能测试
#define VIRTUAL_SERVER_COM "COM4"
#define VIRTUAL_CLIENT_COM "COM5"

TEST(modbus, server_rtu_error) {
  const char server_url[] = "serial://" VIRTUAL_SERVER_COM "?baudrate=9600&stopbits=1&parity=none&flowcontrol=none&bytesize=8";
  const char client_url[] = "serial://" VIRTUAL_CLIENT_COM "?baudrate=9600&stopbits=1&parity=none&flowcontrol=none&bytesize=8";
  test_server_slave_error(server_url, client_url, 0x01, MODBUS_PROTO_RTU);
}
#endif

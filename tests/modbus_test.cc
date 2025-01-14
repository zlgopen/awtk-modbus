
#include "gtest/gtest.h"
#include "modbus_client.h"
#include "modbus_service.h"

#include "streams/mem/iostream_mem.h"

static void check_read_bits_ex(uint16_t func_code, uint8_t value1, uint8_t value2,
                               uint16_t num_bits) {
  uint8_t num_bytes = (num_bits + 7) / 8;

  uint8_t in[1024] = {0,         2,             /*tid*/
                      0,         0,             /*protocol id*/
                      0,         3 + num_bytes, /*data len*/
                      0xff,                     /*Server address (255 if not used)*/
                      (uint8_t)func_code,                /*Function codes as in other variants*/
                      num_bytes, value1,
                      value2};
  uint8_t out[1024] = {0};
  uint8_t buff[1024] = {0};
  tk_iostream_t* io = tk_iostream_mem_create(in, sizeof(in), out, sizeof(out), FALSE);
  tk_iostream_t* io_server = tk_iostream_mem_create(out, sizeof(out), in, sizeof(in), FALSE);
  modbus_client_t* client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  modbus_service_t* service = modbus_service_create_with_io(io_server, MODBUS_PROTO_TCP, NULL);
  modbus_client_set_slave(client, 0xFF);
  modbus_service_set_slave(service, 0xFF);
  if (func_code == MODBUS_FC_READ_COILS) {
    ret_t ret = modbus_client_read_bits(client, 304, num_bits, buff);
    ASSERT_EQ(ret, RET_OK);
  } else {
    ret_t ret = modbus_client_read_input_bits(client, 304, num_bits, buff);
    ASSERT_EQ(ret, RET_OK);
  }

  modbus_req_data_t req;
  ASSERT_EQ(modbus_common_recv_req(&service->common, &req), RET_OK);
  ASSERT_EQ(req.func_code, func_code);
  ASSERT_EQ(req.addr, 304);
  ASSERT_EQ(req.count, num_bits);
  ASSERT_EQ(req.data == NULL, TRUE);

  ASSERT_EQ(out[0], 0);
  ASSERT_EQ(out[1], 2);  //tid

  ASSERT_EQ(out[2], 0);
  ASSERT_EQ(out[3], 0);  //protocol id

  ASSERT_EQ(out[4], 0);
  ASSERT_EQ(out[5], 6);  //data len

  ASSERT_EQ(out[6], 0xff);       //Server address (255 if not used)
  ASSERT_EQ(out[7], func_code);  //Function codes as in other variants

  uint16_t i = 0;
  for (i = 0; i < num_bits; i++) {
    bool_t v = FALSE;
    bits_stream_get(in + 9, 2, i, &v);
    ASSERT_EQ(buff[i], v);
  }
  modbus_client_destroy(client);
  tk_service_destroy((tk_service_t*)service);
}

static void check_read_bits(uint8_t value1, uint8_t value2, uint16_t num_bits) {
  return check_read_bits_ex(MODBUS_FC_READ_COILS, value1, value2, num_bits);
}

static void check_read_input_bits(uint8_t value1, uint8_t value2, uint16_t num_bits) {
  return check_read_bits_ex(MODBUS_FC_READ_DISCRETE_INPUTS, value1, value2, num_bits);
}

TEST(modbus, read_bits_1) {
#if 1
  check_read_bits(0, 0, 1);
  check_read_bits(1, 0, 1);

  check_read_bits(0b01, 0, 2);
  check_read_bits(0b11, 0, 2);

  check_read_bits(0b101, 0, 3);
  check_read_bits(0b011, 0, 3);

  check_read_bits(0b10111101, 0, 8);
#endif
  check_read_bits(0b10111101, 0b01010101, 9);
  check_read_bits(0b10111101, 0b01010101, 10);
  check_read_bits(0b10111101, 0b01010101, 16);
}

TEST(modbus, read_input_bits_1) {
#if 1
  check_read_input_bits(0, 0, 1);
  check_read_input_bits(1, 0, 1);

  check_read_input_bits(0b01, 0, 2);
  check_read_input_bits(0b11, 0, 2);

  check_read_input_bits(0b101, 0, 3);
  check_read_input_bits(0b011, 0, 3);

  check_read_input_bits(0b10111101, 0, 8);
#endif
  check_read_input_bits(0b10111101, 0b01010101, 9);
  check_read_input_bits(0b10111101, 0b01010101, 10);
  check_read_input_bits(0b10111101, 0b01010101, 16);
}

static void check_read_register_ex(uint16_t func_code, uint16_t* data, uint16_t n_registers) {
  uint8_t num_bytes = n_registers * 2;

  uint8_t in[1024] = {0,         2,             /*tid*/
                      0,         0,             /*protocol id*/
                      0,         3 + num_bytes, /*data len*/
                      0xff,                     /*Server address (255 if not used)*/
                      func_code,                /*Function codes as in other variants*/
                      num_bytes};
  uint16_t i = 0;
  for (i = 0; i < n_registers; i++) {
    uint16_t v = data[i];
    in[9 + i * 2] = v >> 8;
    in[9 + i * 2 + 1] = v & 0xff;
  }

  uint8_t out[1024] = {0};
  uint16_t buff[1024] = {0};
  tk_iostream_t* io = tk_iostream_mem_create(in, sizeof(in), out, sizeof(out), FALSE);
  tk_iostream_t* io_server = tk_iostream_mem_create(out, sizeof(out), in, sizeof(in), FALSE);

  modbus_client_t* client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  modbus_service_t* service = modbus_service_create_with_io(io_server, MODBUS_PROTO_TCP, NULL);
  modbus_client_set_slave(client, 0xFF);
  modbus_service_set_slave(service, 0xFF);
  if (func_code == MODBUS_FC_READ_HOLDING_REGISTERS) {
    ret_t ret = modbus_client_read_registers(client, 304, n_registers, buff);
    ASSERT_EQ(ret, RET_OK);

  } else {
    ret_t ret = modbus_client_read_input_registers(client, 304, n_registers, buff);
    ASSERT_EQ(ret, RET_OK);
  }

  modbus_req_data_t req;
  ASSERT_EQ(modbus_common_recv_req(&service->common, &req), RET_OK);
  ASSERT_EQ(req.func_code, func_code);
  ASSERT_EQ(req.addr, 304);
  ASSERT_EQ(req.count, n_registers);
  ASSERT_EQ(req.data == NULL, TRUE);

  ASSERT_EQ(out[0], 0);
  ASSERT_EQ(out[1], 2);  //tid

  ASSERT_EQ(out[2], 0);
  ASSERT_EQ(out[3], 0);  //protocol id

  ASSERT_EQ(out[4], 0);
  ASSERT_EQ(out[5], 6);  //data len

  ASSERT_EQ(out[6], 0xff);       //Server address (255 if not used)
  ASSERT_EQ(out[7], func_code);  //Function codes as in other variants

  for (i = 0; i < n_registers; i++) {
    ASSERT_EQ(buff[i], data[i]);
  }

  modbus_client_destroy(client);
  tk_service_destroy((tk_service_t*)service);
}

TEST(modbus, read_registers_1) {
  uint16_t data[] = {0x1234, 0x5678, 0x9abc, 0xdef0};
  check_read_register_ex(MODBUS_FC_READ_HOLDING_REGISTERS, data, 1);
  check_read_register_ex(MODBUS_FC_READ_HOLDING_REGISTERS, data, 2);
  check_read_register_ex(MODBUS_FC_READ_HOLDING_REGISTERS, data, 3);
  check_read_register_ex(MODBUS_FC_READ_HOLDING_REGISTERS, data, 4);
}

TEST(modbus, read_input_registers_1) {
  uint16_t data[] = {0x1234, 0x5678, 0x9abc, 0xdef0};
  check_read_register_ex(MODBUS_FC_READ_INPUT_REGISTERS, data, 1);
  check_read_register_ex(MODBUS_FC_READ_INPUT_REGISTERS, data, 2);
  check_read_register_ex(MODBUS_FC_READ_INPUT_REGISTERS, data, 3);
  check_read_register_ex(MODBUS_FC_READ_INPUT_REGISTERS, data, 4);
}

static void check_write_bit(uint8_t value1) {
  uint16_t addr = 0x1234;
  uint8_t in[1024] = {0,
                      2, /*tid*/
                      0,
                      0, /*protocol id*/
                      0,
                      4 + 2,                       /*data len*/
                      0xff,                        /*Server address (255 if not used)*/
                      MODBUS_FC_WRITE_SINGLE_COIL, /*Function codes as in other variants*/
                      addr >> 8,
                      addr & 0xff,
                      value1 ? 0xff : 0,
                      0};
  uint8_t out[1024] = {0};
  tk_iostream_t* io = tk_iostream_mem_create(in, sizeof(in), out, sizeof(out), FALSE);
  modbus_client_t* client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  modbus_client_set_slave(client, 0xFF);
  ret_t ret = modbus_client_write_bit(client, addr, value1);
  ASSERT_EQ(ret, RET_OK);

  ASSERT_EQ(out[0], 0);
  ASSERT_EQ(out[1], 2);  //tid

  ASSERT_EQ(out[2], 0);
  ASSERT_EQ(out[3], 0);  //protocol id

  ASSERT_EQ(out[4], 0);
  ASSERT_EQ(out[5], 6);  //data len

  ASSERT_EQ(out[6], 0xff);                         //Server address (255 if not used)
  ASSERT_EQ(out[7], MODBUS_FC_WRITE_SINGLE_COIL);  //Function codes as in other variants
  ASSERT_EQ(out[8], addr >> 8);
  ASSERT_EQ(out[9], addr & 0xff);
  ASSERT_EQ(out[10], value1 ? 0xff : 0);
  ASSERT_EQ(out[11], 0);

  modbus_client_destroy(client);
}

TEST(modbus, write_bit) {
  check_write_bit(0);
  check_write_bit(1);
}

static void check_write_register(uint16_t value1) {
  uint16_t addr = 0x1234;
  uint8_t in[1024] = {
      0,
      2, /*tid*/
      0,
      0, /*protocol id*/
      0,
      4 + 2,                                   /*data len*/
      0xff,                                    /*Server address (255 if not used)*/
      MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER, /*Function codes as in other variants*/
      addr >> 8,
      addr & 0xff,
      value1 >> 8,
      value1 & 0xff};
  uint8_t out[1024] = {0};
  tk_iostream_t* io = tk_iostream_mem_create(in, sizeof(in), out, sizeof(out), FALSE);
  modbus_client_t* client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  modbus_client_set_slave(client, 0xFF);
  ret_t ret = modbus_client_write_register(client, addr, value1);
  ASSERT_EQ(ret, RET_OK);

  ASSERT_EQ(out[0], 0);
  ASSERT_EQ(out[1], 2);  //tid

  ASSERT_EQ(out[2], 0);
  ASSERT_EQ(out[3], 0);  //protocol id

  ASSERT_EQ(out[4], 0);
  ASSERT_EQ(out[5], 6);  //data len

  ASSERT_EQ(out[6], 0xff);                                     //Server address (255 if not used)
  ASSERT_EQ(out[7], MODBUS_FC_WRITE_SINGLE_HOLDING_REGISTER);  //Function codes as in other variants
  ASSERT_EQ(out[8], addr >> 8);
  ASSERT_EQ(out[9], addr & 0xff);
  ASSERT_EQ(out[10], value1 >> 8);
  ASSERT_EQ(out[11], value1 & 0xff);

  modbus_client_destroy(client);
}

TEST(modbus, write_register) {
  check_write_register(0x1234);
  check_write_register(0x5678);
}

static void check_write_bits(uint8_t* value1, uint16_t num_bits) {
  uint16_t addr = 0x1234;
  uint8_t in[1024] = {0,
                      2, /*tid*/
                      0,
                      0, /*protocol id*/
                      0,
                      4 + 2,                          /*data len*/
                      0xff,                           /*Server address (255 if not used)*/
                      MODBUS_FC_WRITE_MULTIPLE_COILS, /*Function codes as in other variants*/
                      addr >> 8,
                      addr & 0xff,
                      num_bits >> 8,
                      num_bits & 0xff};
  uint8_t out[1024] = {0};
  uint8_t bytes = (num_bits + 7) / 8;

  tk_iostream_t* io = tk_iostream_mem_create(in, sizeof(in), out, sizeof(out), FALSE);
  modbus_client_t* client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  modbus_client_set_slave(client, 0xFF);
  ret_t ret = modbus_client_write_bits(client, addr, num_bits, value1);
  ASSERT_EQ(ret, RET_OK);

  ASSERT_EQ(out[0], 0);
  ASSERT_EQ(out[1], 2);  //tid

  ASSERT_EQ(out[2], 0);
  ASSERT_EQ(out[3], 0);  //protocol id

  ASSERT_EQ(out[4], 0);
  ASSERT_EQ(out[5], bytes + 7);  //data len

  ASSERT_EQ(out[6], 0xff);                            //Server address (255 if not used)
  ASSERT_EQ(out[7], MODBUS_FC_WRITE_MULTIPLE_COILS);  //Function codes as in other variants
  ASSERT_EQ(out[8], addr >> 8);
  ASSERT_EQ(out[9], addr & 0xff);
  ASSERT_EQ(out[10], num_bits >> 8);
  ASSERT_EQ(out[11], num_bits & 0xff);
  ASSERT_EQ(out[12], bytes);

  modbus_client_destroy(client);
}

TEST(modbus, write_bits) {
  uint8_t bits[16] = {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0};
  check_write_bits(bits, 1);
  check_write_bits(bits, 6);
  check_write_bits(bits, 16);
}

static void check_write_registers(uint16_t* value1, uint16_t num_registers) {
  uint16_t addr = 0x1234;
  uint8_t in[1024] = {
      0,
      2, /*tid*/
      0,
      0, /*protocol id*/
      0,
      4 + 2,                                      /*data len*/
      0xff,                                       /*Server address (255 if not used)*/
      MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS, /*Function codes as in other variants*/
      addr >> 8,
      addr & 0xff,
      num_registers >> 8,
      num_registers & 0xff};
  uint8_t out[1024] = {0};
  uint8_t bytes = num_registers * 2;

  tk_iostream_t* io = tk_iostream_mem_create(in, sizeof(in), out, sizeof(out), FALSE);
  modbus_client_t* client = modbus_client_create_with_io(io, MODBUS_PROTO_TCP);
  modbus_client_set_slave(client, 0xFF);
  ret_t ret = modbus_client_write_registers(client, addr, num_registers, value1);
  ASSERT_EQ(ret, RET_OK);

  ASSERT_EQ(out[0], 0);
  ASSERT_EQ(out[1], 2);  //tid

  ASSERT_EQ(out[2], 0);
  ASSERT_EQ(out[3], 0);  //protocol id

  ASSERT_EQ(out[4], 0);
  ASSERT_EQ(out[5], bytes + 7);  //data len

  ASSERT_EQ(out[6], 0xff);  //Server address (255 if not used)
  ASSERT_EQ(out[7],
            MODBUS_FC_WRITE_MULTIPLE_HOLDING_REGISTERS);  //Function codes as in other variants
  ASSERT_EQ(out[8], addr >> 8);
  ASSERT_EQ(out[9], addr & 0xff);
  ASSERT_EQ(out[10], num_registers >> 8);
  ASSERT_EQ(out[11], num_registers & 0xff);
  ASSERT_EQ(out[12], bytes);

  modbus_client_destroy(client);
}

TEST(modbus, write_registers) {
  uint16_t data[] = {0x1234, 0x5678, 0x9abc, 0xdef0};
  check_write_registers(data, 1);
  check_write_registers(data, 2);
  check_write_registers(data, 3);
  check_write_registers(data, 4);
}

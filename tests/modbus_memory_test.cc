#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "modbus_memory_default.h"

#define BITS_START MODBUS_DEMO_BITS_ADDRESS
#define INPUT_BITS_START MODBUS_DEMO_INPUT_BITS_ADDRESS
#define REGISTERS_START MODBUS_DEMO_REGISTERS_ADDRESS
#define INPUT_REGISTERS_START MODBUS_DEMO_INPUT_REGISTERS_ADDRESS

#define BITS_COUNT MODBUS_DEMO_BITS_NB
#define INPUT_BITS_COUNT MODBUS_DEMO_INPUT_BITS_NB
#define REGISTERS_COUNT MODBUS_DEMO_REGISTERS_NB
#define INPUT_REGISTERS_COUNT MODBUS_DEMO_INPUT_BITS_NB

TEST(modbus, memory) {
  modbus_memory_t* memory = modbus_memory_default_create_test();
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  ASSERT_EQ(memory_default->bits->start, BITS_START);
  ASSERT_EQ(memory_default->bits->length, BITS_COUNT);
  ASSERT_EQ(memory_default->input_bits->start, INPUT_BITS_START);
  ASSERT_EQ(memory_default->input_bits->length, INPUT_BITS_COUNT);
  ASSERT_EQ(memory_default->registers->start, REGISTERS_START);
  ASSERT_EQ(memory_default->registers->length, REGISTERS_COUNT);
  ASSERT_EQ(memory_default->input_registers->start, INPUT_REGISTERS_START);
  ASSERT_EQ(memory_default->input_registers->length, INPUT_REGISTERS_COUNT);

  ASSERT_EQ(memory_default->bits->data != NULL, TRUE);
  ASSERT_EQ(memory_default->input_bits->data != NULL, TRUE);
  ASSERT_EQ(memory_default->registers->data != NULL, TRUE);
  ASSERT_EQ(memory_default->input_registers->data != NULL, TRUE);

  uint8_t bits_data[MODBUS_DEMO_BITS_NB+1];
  uint8_t input_bits_data[MODBUS_DEMO_INPUT_BITS_NB+1];
  uint16_t registers_data[100];
  uint16_t input_registers_data[100];

  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, 0, bits_data), RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, MODBUS_MAX_READ_BITS + 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, BITS_COUNT + 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START - 1, 1, bits_data), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, BITS_COUNT, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START, 0, bits_data), RET_BAD_PARAMS);
  ASSERT_EQ(
      modbus_memory_read_input_bits(memory, INPUT_BITS_START, MODBUS_MAX_READ_BITS + 1, bits_data),
      RET_INVALID_ADDR);
  ASSERT_EQ(
      modbus_memory_read_input_bits(memory, INPUT_BITS_START, INPUT_BITS_COUNT + 1, bits_data),
      RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START - 1, 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START, INPUT_BITS_COUNT, NULL),
            RET_BAD_PARAMS);

  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, 0, registers_data),
            RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, MODBUS_MAX_READ_REGISTERS + 1,
                                         registers_data),
            RET_INVALID_ADDR);

  ASSERT_EQ(
      modbus_memory_read_registers(memory, REGISTERS_START, REGISTERS_COUNT + 1, registers_data),
      RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START - 1, 1, registers_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, REGISTERS_COUNT, NULL),
            RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_read_input_registers(memory, INPUT_REGISTERS_START,
                                               INPUT_REGISTERS_COUNT + 1, registers_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(
      modbus_memory_read_input_registers(memory, INPUT_REGISTERS_START - 1, 1, registers_data),
      RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_input_registers(memory, INPUT_REGISTERS_START, INPUT_REGISTERS_COUNT,
                                               NULL),
            RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, 0, bits_data), RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, MODBUS_MAX_WRITE_BITS + 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, BITS_COUNT + 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START - 1, 1, bits_data), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, BITS_COUNT, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, 0, registers_data),
            RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, MODBUS_MAX_WRITE_REGISTERS + 1,
                                          registers_data),
            RET_INVALID_ADDR);

  ASSERT_EQ(
      modbus_memory_write_registers(memory, REGISTERS_START, REGISTERS_COUNT + 1, registers_data),
      RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START - 1, 1, registers_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, REGISTERS_COUNT, NULL),
            RET_BAD_PARAMS);
  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START - 1, 2), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START + BITS_COUNT, 2), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START - 1, 2), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START + REGISTERS_COUNT, 2),
            RET_INVALID_ADDR);

  memset(bits_data, 0, sizeof(bits_data));
  bits_data[0] = 0b01010101;
  bits_data[1] = 0b01010101;

  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, BITS_COUNT, bits_data), RET_OK);

  bool_t v = 0;
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, BITS_COUNT, bits_data), RET_OK);
  ASSERT_EQ(bits_stream_get(bits_data, 10, 0, &v), RET_OK);
  ASSERT_EQ(v, 1);
  ASSERT_EQ(bits_stream_get(bits_data, 10, 1, &v), RET_OK);
  ASSERT_EQ(v, 0);
  ASSERT_EQ(bits_stream_get(bits_data, 10, 2, &v), RET_OK);
  ASSERT_EQ(v, 1);
  ASSERT_EQ(bits_stream_get(bits_data, 10, 3, &v), RET_OK);
  ASSERT_EQ(v, 0);
  ASSERT_EQ(bits_stream_get(bits_data, 10, 4, &v), RET_OK);
  ASSERT_EQ(v, 1);
  ASSERT_EQ(bits_stream_get(bits_data, 10, 5, &v), RET_OK);
  ASSERT_EQ(v, 0);

  memory_default->bits->data[0] = 0;
  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START + 1, 1), RET_OK);
  ASSERT_EQ(memory_default->bits->data[0], 0b10);
  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START + 5, 1), RET_OK);
  ASSERT_EQ(memory_default->bits->data[0], 0b00100010);

  ((uint16_t*)(memory_default->input_bits->data))[0] = 0x0505;
  ((uint16_t*)(memory_default->input_bits->data))[1] = 0x0505;

  v = 0;
  ASSERT_EQ(
      modbus_memory_read_input_bits(memory, INPUT_BITS_START, INPUT_BITS_COUNT, input_bits_data),
      RET_OK);
  ASSERT_EQ(bits_stream_get(input_bits_data, 10, 0, &v), RET_OK);
  ASSERT_EQ(v, 1);
  ASSERT_EQ(bits_stream_get(input_bits_data, 10, 1, &v), RET_OK);
  ASSERT_EQ(v, 0);
  ASSERT_EQ(bits_stream_get(input_bits_data, 10, 2, &v), RET_OK);
  ASSERT_EQ(v, 1);
  ASSERT_EQ(bits_stream_get(input_bits_data, 10, 3, &v), RET_OK);
  ASSERT_EQ(v, 0);
  ASSERT_EQ(bits_stream_get(input_bits_data, 10, 4, &v), RET_OK);
  //ASSERT_EQ(v, 1);
  //ASSERT_EQ(bits_stream_get(input_bits_data, 10, 5, &v), RET_OK);
  //ASSERT_EQ(v, 0);

  ((uint16_t*)memory_default->registers->data)[0] = 0x1234;
  ((uint16_t*)memory_default->registers->data)[1] = 0x5678;
  ((uint16_t*)memory_default->registers->data)[2] = 0x9abc;
  ((uint16_t*)memory_default->registers->data)[3] = 0xdef0;
  ((uint16_t*)memory_default->registers->data)[4] = 0x1234;
  ((uint16_t*)memory_default->registers->data)[5] = 0x5678;

  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, MODBUS_MAX_READ_REGISTERS, registers_data),
            RET_OK);
  ASSERT_EQ(uint16_from_big_endian(registers_data[0]), 0x1234);
  ASSERT_EQ(uint16_from_big_endian(registers_data[1]), 0x5678);
  ASSERT_EQ(uint16_from_big_endian(registers_data[2]), 0x9abc);
  ASSERT_EQ(uint16_from_big_endian(registers_data[3]), 0xdef0);
  ASSERT_EQ(uint16_from_big_endian(registers_data[4]), 0x1234);
  ASSERT_EQ(uint16_from_big_endian(registers_data[5]), 0x5678);

  registers_data[0] = 0x1234;
  registers_data[1] = 0x5678;
  registers_data[2] = 0x9abc;
  registers_data[3] = 0xdef0;
  registers_data[4] = 0x1234;
  registers_data[5] = 0x5678;

  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, 6, registers_data), RET_OK);

  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[0]), 0x1234);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[1]), 0x5678);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[2]), 0x9abc);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[3]), 0xdef0);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[4]), 0x1234);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[5]), 0x5678);

  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START + 1, 0x1234), RET_OK);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[1]), 0x1234);

  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START + 5, 0x1234), RET_OK);
  ASSERT_EQ(uint16_from_big_endian(((uint16_t*)memory_default->registers->data)[5]), 0x1234);

  ((uint16_t*)memory_default->input_registers->data)[0] = 0x1234;
  ((uint16_t*)memory_default->input_registers->data)[1] = 0x5678;
  ((uint16_t*)memory_default->input_registers->data)[2] = 0x9abc;
  ((uint16_t*)memory_default->input_registers->data)[3] = 0xdef0;
  ((uint16_t*)memory_default->input_registers->data)[4] = 0x1234;
  ((uint16_t*)memory_default->input_registers->data)[5] = 0x5678;
  ASSERT_EQ(modbus_memory_read_input_registers(memory, INPUT_REGISTERS_START, MODBUS_MAX_READ_REGISTERS,
                                               input_registers_data),
            RET_OK);
  ASSERT_EQ(uint16_from_big_endian(input_registers_data[0]), 0x1234);
  ASSERT_EQ(uint16_from_big_endian(input_registers_data[1]), 0x5678);
  ASSERT_EQ(uint16_from_big_endian(input_registers_data[2]), 0x9abc);
  ASSERT_EQ(uint16_from_big_endian(input_registers_data[3]), 0xdef0);
  ASSERT_EQ(uint16_from_big_endian(input_registers_data[4]), 0x1234);
  ASSERT_EQ(uint16_from_big_endian(input_registers_data[5]), 0x5678);

  modbus_memory_destroy(memory);
}

typedef struct _memory_hooks_ctx_t {
  int before_read_bits_count;
  int before_read_input_bits_count;
  int before_read_registers_count;
  int before_read_input_registers_count;
  int after_write_bit_count;
  int after_write_bits_count;
  int after_write_register_count;
  int after_write_registers_count;
  uint16_t last_addr;
  uint16_t last_count;
} memory_hooks_ctx_t;

static ret_t on_before_read_bits(void* ctx, uint16_t addr, uint16_t count) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->before_read_bits_count++;
  c->last_addr = addr;
  c->last_count = count;
  return RET_OK;
}

static ret_t on_before_read_input_bits(void* ctx, uint16_t addr, uint16_t count) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->before_read_input_bits_count++;
  c->last_addr = addr;
  c->last_count = count;
  return RET_OK;
}

static ret_t on_before_read_registers(void* ctx, uint16_t addr, uint16_t count) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->before_read_registers_count++;
  c->last_addr = addr;
  c->last_count = count;
  return RET_FAIL;
}

static ret_t on_before_read_input_registers(void* ctx, uint16_t addr, uint16_t count) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->before_read_input_registers_count++;
  c->last_addr = addr;
  c->last_count = count;
  return RET_OK;
}

static ret_t on_after_write_bit(void* ctx, uint16_t addr) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->after_write_bit_count++;
  c->last_addr = addr;
  c->last_count = 1;
  return RET_OK;
}

static ret_t on_after_write_bits(void* ctx, uint16_t addr, uint16_t count) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->after_write_bits_count++;
  c->last_addr = addr;
  c->last_count = count;
  return RET_OK;
}

static ret_t on_after_write_register(void* ctx, uint16_t addr) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->after_write_register_count++;
  c->last_addr = addr;
  c->last_count = 1;
  return RET_FAIL;
}

static ret_t on_after_write_registers(void* ctx, uint16_t addr, uint16_t count) {
  memory_hooks_ctx_t* c = (memory_hooks_ctx_t*)ctx;
  c->after_write_registers_count++;
  c->last_addr = addr;
  c->last_count = count;
  return RET_OK;
}

TEST(modbus, memory_default_hooks) {
  uint8_t bits_data[2] = {0};
  uint16_t registers_data[2] = {0x1234, 0x5678};
  modbus_memory_t* memory = modbus_memory_default_create_test();
  memory_hooks_ctx_t hooks_ctx = {0};
  modbus_memory_default_hooks_t hooks = {0};

  hooks.ctx = &hooks_ctx;
  hooks.before_read_bits = on_before_read_bits;
  hooks.before_read_input_bits = on_before_read_input_bits;
  hooks.before_read_registers = on_before_read_registers;
  hooks.before_read_input_registers = on_before_read_input_registers;
  hooks.after_write_bit = on_after_write_bit;
  hooks.after_write_bits = on_after_write_bits;
  hooks.after_write_register = on_after_write_register;
  hooks.after_write_registers = on_after_write_registers;
  ASSERT_EQ(modbus_memory_default_set_hooks(memory, &hooks), RET_OK);

  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, 2, bits_data), RET_OK);
  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START, 2, bits_data), RET_OK);
  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, 2, registers_data), RET_OK);
  ASSERT_EQ(modbus_memory_read_input_registers(memory, INPUT_REGISTERS_START, 2, registers_data),
            RET_OK);
  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START, 1), RET_OK);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, 2, bits_data), RET_OK);
  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START, 0x1234), RET_OK);
  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, 2, registers_data), RET_OK);

  ASSERT_EQ(hooks_ctx.before_read_bits_count, 1);
  ASSERT_EQ(hooks_ctx.before_read_input_bits_count, 1);
  ASSERT_EQ(hooks_ctx.before_read_registers_count, 1);
  ASSERT_EQ(hooks_ctx.before_read_input_registers_count, 1);
  ASSERT_EQ(hooks_ctx.after_write_bit_count, 1);
  ASSERT_EQ(hooks_ctx.after_write_bits_count, 1);
  ASSERT_EQ(hooks_ctx.after_write_register_count, 1);
  ASSERT_EQ(hooks_ctx.after_write_registers_count, 1);
  ASSERT_EQ(hooks_ctx.last_addr, REGISTERS_START);
  ASSERT_EQ(hooks_ctx.last_count, 2);

  ASSERT_EQ(modbus_memory_default_set_hooks(memory, NULL), RET_OK);
  modbus_memory_destroy(memory);
}

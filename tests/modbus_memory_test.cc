#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "modbus_memory_default.h"

#define BITS_START 0
#define INPUT_BITS_START 10000
#define REGISTERS_START 30000
#define INPUT_REGISTERS_START 40000

#define BITS_COUNT 10
#define INPUT_BITS_COUNT 10
#define REGISTERS_COUNT 10
#define INPUT_REGISTERS_COUNT 10

TEST(modbus, memory) {
  modbus_memory_t* memory = modbus_memory_default_create(
      BITS_START, BITS_COUNT, INPUT_BITS_START, INPUT_BITS_COUNT, REGISTERS_START, REGISTERS_COUNT,
      INPUT_REGISTERS_START, INPUT_REGISTERS_COUNT);
  modbus_memory_default_t* memory_default = MODBUS_MEMORY_DEFAULT(memory);
  ASSERT_EQ(memory_default->bits_start, BITS_START);
  ASSERT_EQ(memory_default->bits_count, BITS_COUNT);
  ASSERT_EQ(memory_default->input_bits_start, INPUT_BITS_START);
  ASSERT_EQ(memory_default->input_bits_count, INPUT_BITS_COUNT);
  ASSERT_EQ(memory_default->registers_start, REGISTERS_START);
  ASSERT_EQ(memory_default->registers_count, REGISTERS_COUNT);
  ASSERT_EQ(memory_default->input_registers_start, INPUT_REGISTERS_START);
  ASSERT_EQ(memory_default->input_registers_count, INPUT_REGISTERS_COUNT);

  ASSERT_EQ(memory_default->bits_data != NULL, TRUE);
  ASSERT_EQ(memory_default->input_bits_data != NULL, TRUE);
  ASSERT_EQ(memory_default->registers_data != NULL, TRUE);
  ASSERT_EQ(memory_default->input_registers_data != NULL, TRUE);

  uint8_t bits_data[100];
  uint8_t input_bits_data[100];
  uint16_t registers_data[100];
  uint16_t input_registers_data[100];

  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, 0, bits_data), RET_EXCEED_RANGE);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, MODBUS_MAX_READ_BITS + 1, bits_data),
            RET_EXCEED_RANGE);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, BITS_COUNT + 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START - 1, 1, bits_data), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_bits(memory, BITS_START, BITS_COUNT, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START, 0, bits_data),
            RET_EXCEED_RANGE);
  ASSERT_EQ(
      modbus_memory_read_input_bits(memory, INPUT_BITS_START, MODBUS_MAX_READ_BITS + 1, bits_data),
      RET_EXCEED_RANGE);
  ASSERT_EQ(
      modbus_memory_read_input_bits(memory, INPUT_BITS_START, INPUT_BITS_COUNT + 1, bits_data),
      RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START - 1, 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_read_input_bits(memory, INPUT_BITS_START, INPUT_BITS_COUNT, NULL),
            RET_BAD_PARAMS);

  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, 0, registers_data),
            RET_EXCEED_RANGE);
  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, MODBUS_MAX_READ_REGISTERS + 1,
                                         registers_data),
            RET_EXCEED_RANGE);

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
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, 0, bits_data), RET_EXCEED_RANGE);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, MODBUS_MAX_WRITE_BITS + 1, bits_data),
            RET_EXCEED_RANGE);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, BITS_COUNT + 1, bits_data),
            RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START - 1, 1, bits_data), RET_INVALID_ADDR);
  ASSERT_EQ(modbus_memory_write_bits(memory, BITS_START, BITS_COUNT, NULL), RET_BAD_PARAMS);

  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, 0, registers_data),
            RET_EXCEED_RANGE);
  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, MODBUS_MAX_WRITE_REGISTERS + 1,
                                          registers_data),
            RET_EXCEED_RANGE);

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

  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START + 1, 1), RET_OK);
  ASSERT_EQ(memory_default->bits_data[1], 1);
  ASSERT_EQ(modbus_memory_write_bit(memory, BITS_START + 5, 1), RET_OK);
  ASSERT_EQ(memory_default->bits_data[5], 1);

  memory_default->input_bits_data[0] = 1;
  memory_default->input_bits_data[1] = 0;
  memory_default->input_bits_data[2] = 1;
  memory_default->input_bits_data[3] = 0;
  memory_default->input_bits_data[4] = 1;
  memory_default->input_bits_data[5] = 0;

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
  ASSERT_EQ(v, 1);
  ASSERT_EQ(bits_stream_get(input_bits_data, 10, 5, &v), RET_OK);
  ASSERT_EQ(v, 0);

  memory_default->registers_data[0] = 0x1234;
  memory_default->registers_data[1] = 0x5678;
  memory_default->registers_data[2] = 0x9abc;
  memory_default->registers_data[3] = 0xdef0;
  memory_default->registers_data[4] = 0x1234;
  memory_default->registers_data[5] = 0x5678;

  ASSERT_EQ(modbus_memory_read_registers(memory, REGISTERS_START, REGISTERS_COUNT, registers_data),
            RET_OK);
  ASSERT_EQ(registers_data[0], 0x3412);
  ASSERT_EQ(registers_data[1], 0x7856);
  ASSERT_EQ(registers_data[2], 0xbc9a);
  ASSERT_EQ(registers_data[3], 0xf0de);
  ASSERT_EQ(registers_data[4], 0x3412);
  ASSERT_EQ(registers_data[5], 0x7856);

  registers_data[0] = 0x1234;
  registers_data[1] = 0x5678;
  registers_data[2] = 0x9abc;
  registers_data[3] = 0xdef0;
  registers_data[4] = 0x1234;
  registers_data[5] = 0x5678;

  ASSERT_EQ(modbus_memory_write_registers(memory, REGISTERS_START, 6, registers_data), RET_OK);

  ASSERT_EQ(memory_default->registers_data[0], 0x3412);
  ASSERT_EQ(memory_default->registers_data[1], 0x7856);
  ASSERT_EQ(memory_default->registers_data[2], 0xbc9a);
  ASSERT_EQ(memory_default->registers_data[3], 0xf0de);
  ASSERT_EQ(memory_default->registers_data[4], 0x3412);
  ASSERT_EQ(memory_default->registers_data[5], 0x7856);

  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START + 1, 0x1234), RET_OK);
  ASSERT_EQ(memory_default->registers_data[1], 0x3412);

  ASSERT_EQ(modbus_memory_write_register(memory, REGISTERS_START + 5, 0x1234), RET_OK);
  ASSERT_EQ(memory_default->registers_data[5], 0x3412);

  memory_default->input_registers_data[0] = 0x1234;
  memory_default->input_registers_data[1] = 0x5678;
  memory_default->input_registers_data[2] = 0x9abc;
  memory_default->input_registers_data[3] = 0xdef0;
  memory_default->input_registers_data[4] = 0x1234;
  memory_default->input_registers_data[5] = 0x5678;
  ASSERT_EQ(modbus_memory_read_input_registers(memory, INPUT_REGISTERS_START, INPUT_REGISTERS_COUNT,
                                               input_registers_data),
            RET_OK);
  ASSERT_EQ(input_registers_data[0], 0x3412);
  ASSERT_EQ(input_registers_data[1], 0x7856);
  ASSERT_EQ(input_registers_data[2], 0xbc9a);
  ASSERT_EQ(input_registers_data[3], 0xf0de);
  ASSERT_EQ(input_registers_data[4], 0x3412);
  ASSERT_EQ(input_registers_data[5], 0x7856);

  modbus_memory_destroy(memory);
}

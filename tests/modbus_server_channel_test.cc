#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "modbus_server_channel.h"


TEST(modbus, server_channel_bits) {
    uint32_t i = 0;
    uint8_t buff[16];
    uint8_t data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0xa1, 0xa2, 0xb3, 0xc4, 0xd5, 0xe6};
    uint32_t bits = sizeof(data) * 8;
    modbus_server_channel_t *channel = modbus_server_channel_create("bits", 100, 300, TRUE);
    ASSERT_TRUE(channel != NULL);

    for(i = 0; i < bits; i++) {
      bool_t v =  FALSE;
      bits_stream_get(data, sizeof(data), i, &v);
      ASSERT_EQ(modbus_server_channel_write_bit(channel, channel->start + i, v), RET_OK);
    }
    ASSERT_EQ(memcmp(channel->data, data, sizeof(data)), 0);

    ASSERT_EQ(modbus_server_channel_read_bits(channel, channel->start, bits, buff), RET_OK);
    ASSERT_EQ(memcmp(data, buff, sizeof(data)), 0);

    memset(channel->data, 0, channel->bytes);
    ASSERT_EQ(modbus_server_channel_write_bits(channel, channel->start, bits, data), RET_OK);
    ASSERT_EQ(modbus_server_channel_read_bits(channel, channel->start, bits, buff), RET_OK);
    ASSERT_EQ(memcmp(data, buff, sizeof(data)), 0);
    
    for(i = 0; i < sizeof(data); i++) {
      ASSERT_EQ(modbus_server_channel_read_bits(channel, channel->start + 8 * i, 8, buff), RET_OK);
      ASSERT_EQ(memcmp(data+i, buff, 1), 0);
    }

    modbus_server_channel_destroy(channel);
}

TEST(modbus, server_channel_registers) {
    uint32_t i = 0;
    uint16_t buff[16];
    uint16_t data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0xa121, 0xa212, 0xb331, 0xc411, 0xd532, 0xe612};
    uint32_t registers = ARRAY_SIZE(data);
    modbus_server_channel_t *channel = modbus_server_channel_create("registers", 100, 300, TRUE);

    for(i = 0; i < registers; i++) {
      ASSERT_EQ(modbus_server_channel_write_register(channel, channel->start + i, data[i]), RET_OK);
      ASSERT_EQ(*(((uint16_t *)channel->data) + i), int16_to_big_endian(data[i]));
    }

    ASSERT_EQ(modbus_server_channel_read_registers(channel, channel->start, registers, buff), RET_OK);
    ASSERT_EQ(memcmp(data, buff, sizeof(data)), 0);

    memset(channel->data, 0, channel->bytes);
    ASSERT_EQ(modbus_server_channel_write_registers(channel, channel->start, registers, data), RET_OK);
    ASSERT_EQ(modbus_server_channel_read_registers(channel, channel->start, registers, buff), RET_OK);
    ASSERT_EQ(memcmp(data, buff, sizeof(data)), 0);

    for(i = 0; i < registers; i++) {
      ASSERT_EQ(modbus_server_channel_read_registers(channel, channel->start + i, 1, buff), RET_OK);
      ASSERT_EQ(data[i], buff[0]);
    }

    ASSERT_TRUE(channel != NULL);
    modbus_server_channel_destroy(channel);
}

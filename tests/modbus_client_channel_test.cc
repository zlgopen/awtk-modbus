#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "conf_io/conf_json.h"
#include "modbus_client_channel.h"


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
  conf_doc_destroy(conf);
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
  conf_doc_destroy(conf);
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
  conf_doc_destroy(conf);
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
  conf_doc_destroy(conf);
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

  modbus_client_channel_destroy(channel);
  conf_doc_destroy(conf);
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

  modbus_client_channel_destroy(channel);
  conf_doc_destroy(conf);
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

  conf_doc_destroy(conf);
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

  conf_doc_destroy(conf);
}

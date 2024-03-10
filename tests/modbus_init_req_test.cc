#include "gtest/gtest.h"
#include "tkc/utils.h"
#include "conf_io/conf_json.h"
#include "modbus_init_req.h"

TEST(modbus_init_req, init5) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/init_5.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);
  return_if_fail(conf);
  modbus_init_req_t* init = modbus_init_req_create(conf->root);

  ASSERT_EQ(init->access_type, 5);
  ASSERT_EQ(init->unit_id, 2);
  ASSERT_EQ(init->offset, 0x138);
  ASSERT_EQ(init->length, 0);
  ASSERT_EQ(init->value, 1);

  modbus_init_req_destroy(init);
  TK_OBJECT_UNREF(obj);
}

TEST(modbus_init_req, init15) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/init_15.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);
  return_if_fail(conf);
  modbus_init_req_t* init = modbus_init_req_create(conf->root);

  ASSERT_EQ(init->access_type, 15);
  ASSERT_EQ(init->unit_id, 0);
  ASSERT_EQ(init->offset, 0x130);
  ASSERT_EQ(init->length, 8);
  ASSERT_EQ(init->value, 1);

  modbus_init_req_destroy(init);
  TK_OBJECT_UNREF(obj);
}

TEST(modbus_init_req, init6) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/init_6.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);
  return_if_fail(conf);
  modbus_init_req_t* init = modbus_init_req_create(conf->root);

  ASSERT_EQ(init->access_type, 6);
  ASSERT_EQ(init->unit_id, 0);
  ASSERT_EQ(init->offset, 0x160);
  ASSERT_EQ(init->length, 1);
  ASSERT_EQ(init->value, 100);

  modbus_init_req_destroy(init);
  TK_OBJECT_UNREF(obj);
}


TEST(modbus_init_req, init16) {
  tk_object_t* obj = conf_json_load("file://./tests/testdata/16.json", FALSE);
  conf_doc_t* conf = conf_obj_get_doc(obj);
  return_if_fail(conf);
  modbus_init_req_t* init = modbus_init_req_create(conf->root);

  ASSERT_EQ(init->access_type, 16);
  ASSERT_EQ(init->unit_id, 0);
  ASSERT_EQ(init->offset, 0x164);
  ASSERT_EQ(init->length, 8);
  ASSERT_EQ(init->value, 500);

  modbus_init_req_destroy(init);
  TK_OBJECT_UNREF(obj);
}


#include "tkc/fs.h"
#include "tkc/path.h"
#include "modbus_app.h"

static ret_t tkc_fs_test_dir(const char* path) {
  if (path_exist(path)) {
    fs_remove_dir_r(os_fs(), path);
  }

  fs_create_dir_r(os_fs(), path);
  assert(path_exist(path) == TRUE);

  fs_remove_dir_r(os_fs(), path);
  assert(path_exist(path) == FALSE);

  return RET_OK;
}

static ret_t tkc_fs_test_file(const char* filename) {
  uint32_t size = 0;
  char* data = NULL;

  const char* content = "hello_world";
  file_write(filename, content, strlen(content));

  data = file_read(filename, &size);
  assert(data != NULL);
  assert(size == strlen(content));
  assert(strcmp(data, content) == 0);

  return RET_OK;
}

ret_t tkc_fs_test(void) {
  tkc_fs_test_file("0:/test.txt");
  tkc_fs_test_file("1:/test.txt");

  tkc_fs_test_dir("0:/test/foo");
  tkc_fs_test_dir("1:/test/foo");

  return RET_OK;
}

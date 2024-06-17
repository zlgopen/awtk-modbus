set(AWTK_MODBUS_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Path to AWTK_MODBUS root directory")

# 查找awtk库的头文件和库文件
find_library(AWTK_MODBUS_LIBRARY NAMES modbus PATHS ${AWTK_MODBUS_ROOT_DIR}/bin)

file(COPY  ${AWTK_MODBUS_LIBRARY} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

set(AWTK_MODBUS_LIBRARIES ${AWTK_MODBUS_LIBRARY})
set(AWTK_MODBUS_INCLUDE_DIRS ${AWTK_MODBUS_ROOT_DIR}/src)

link_directories(${AWTK_MODBUS_ROOT_DIR}/bin)
include_directories(${AWTK_MODBUS_INCLUDE_DIRS})

if(WIN32)
  file(COPY  ${AWTK_MODBUS_ROOT_DIR}/bin/modbus.dll DESTINATION ${CMAKE_BINARY_DIR}/Debug)
  file(COPY  ${AWTK_MODBUS_ROOT_DIR}/bin/modbus.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin)
endif()

cmake_minimum_required(VERSION 3.16)

project(awtk-modbus
  VERSION 1.0.0
  LANGUAGES C
  DESCRIPTION "Modbus library for AWTK"
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

find_package(awtk CONFIG REQUIRED)

file(GLOB AWTK_MODBUS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(modbus SHARED ${AWTK_MODBUS_SOURCES})

set_target_properties(modbus PROPERTIES
  OUTPUT_NAME modbus
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(WIN32)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/modbus.def")
    target_sources(modbus PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/modbus.def")
  else()
    # SCons expects src/modbus.def but it may be absent from VCS; MSVC needs exports or there is no modbus.lib.
    set_target_properties(modbus PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  endif()
endif()

target_include_directories(modbus PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/awtk-modbus>"
)

target_link_libraries(modbus PUBLIC awtk::awtk)

# log.h (MSVC) expands log_debug to printf; ensure the UCRT stdio compatibility
# symbols are linked when building modbus.dll (avoids LNK2001 printf).
if(MSVC)
  target_link_libraries(modbus PRIVATE legacy_stdio_definitions)
endif()

if(APPLE)
  set(_modbus_install_rpath "@loader_path")
elseif(UNIX)
  set(_modbus_install_rpath "$ORIGIN")
else()
  set(_modbus_install_rpath "")
endif()

if(UNIX)
  set_target_properties(modbus PROPERTIES
    INSTALL_RPATH "${_modbus_install_rpath}"
    BUILD_WITH_INSTALL_RPATH FALSE
    MACOSX_RPATH ON
  )
endif()

include(CMakePackageConfigHelpers)

set(AWTK_MODBUS_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/awtk-modbus")

install(
  TARGETS modbus
  EXPORT awtk_modbus_export
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/awtk-modbus"
  FILES_MATCHING PATTERN "*.h"
)

install(
  EXPORT awtk_modbus_export
  FILE awtk-modbusTargets.cmake
  NAMESPACE awtk_modbus::
  DESTINATION "${AWTK_MODBUS_CMAKE_DIR}"
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/awtk-modbusConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/awtk-modbusConfig.cmake"
  INSTALL_DESTINATION "${AWTK_MODBUS_CMAKE_DIR}"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/awtk-modbusConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/awtk-modbusConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/awtk-modbusConfigVersion.cmake"
  DESTINATION "${AWTK_MODBUS_CMAKE_DIR}"
)

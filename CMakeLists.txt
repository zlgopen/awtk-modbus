cmake_minimum_required(VERSION 3.16)

project(awtk-modbus
  VERSION 1.0.0
  LANGUAGES C CXX
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

# awtk applies HAS_STDIO / platform macros as PRIVATE defs, which are NOT propagated.
# Without them:
# - types_def.h never enables WITH_SOCKET → modbus_service_tcp_start() is RET_NOT_IMPL
# - log.h log_warn/log_debug become no-ops → mismatch diagnostics never print
target_compile_definitions(modbus PRIVATE HAS_STDIO HAVE_STDIO_H)
if(APPLE)
  target_compile_definitions(modbus PRIVATE MACOS=1)
elseif(UNIX)
  target_compile_definitions(modbus PRIVATE LINUX=1)
elseif(WIN32)
  target_compile_definitions(modbus PRIVATE WIN32=1 WINDOWS=1)
endif()

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

option(AWTK_MODBUS_BUILD_DEMOS "Build demo programs" ON)

if(AWTK_MODBUS_BUILD_DEMOS)
  set(_awtk_modbus_demos_dir "${CMAKE_CURRENT_SOURCE_DIR}/demos")
  set(_awtk_modbus_demo_runtime_dir "${CMAKE_BINARY_DIR}/bin")

  function(_awtk_modbus_add_cli_demo target)
    add_executable(${target} ${ARGN})
    target_include_directories(${target} PRIVATE "${_awtk_modbus_demos_dir}")
    target_link_libraries(${target} PRIVATE modbus)
    set_target_properties(${target} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${_awtk_modbus_demo_runtime_dir}"
    )
    if(MSVC)
      target_link_libraries(${target} PRIVATE legacy_stdio_definitions)
    endif()
    if(UNIX)
      set_target_properties(${target} PROPERTIES
        INSTALL_RPATH "${_modbus_install_rpath}"
        BUILD_WITH_INSTALL_RPATH FALSE
        MACOSX_RPATH ON
      )
    endif()
  endfunction()

  _awtk_modbus_add_cli_demo(modbus_client "${_awtk_modbus_demos_dir}/client.c")
  _awtk_modbus_add_cli_demo(modbus_server
    "${_awtk_modbus_demos_dir}/server.c"
    "${_awtk_modbus_demos_dir}/modbus_memory_custom.c"
  )
  _awtk_modbus_add_cli_demo(modbus_server_ex "${_awtk_modbus_demos_dir}/server_ex.c")

  # modbus_server_awtk mirrors awtk/demos/CMakeLists.txt (demoui + assets static lib).
  set(AWTK_MODBUS_AWTK_SOURCE "" CACHE PATH
    "AWTK source tree; used by update_res.py when res/assets.inc is missing")

  if(NOT AWTK_MODBUS_AWTK_SOURCE)
    foreach(_awtk_source_candidate IN ITEMS
        "${CMAKE_CURRENT_SOURCE_DIR}/../awtk"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../awtk")
      if(EXISTS "${_awtk_source_candidate}/scripts/update_res_app.py")
        set(AWTK_MODBUS_AWTK_SOURCE "${_awtk_source_candidate}" CACHE PATH
          "AWTK source tree" FORCE)
        break()
      endif()
    endforeach()
  endif()

  set(_awtk_modbus_assets_inc "${CMAKE_CURRENT_SOURCE_DIR}/res/assets.inc")
  if(AWTK_MODBUS_AWTK_SOURCE AND NOT EXISTS "${_awtk_modbus_assets_inc}")
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_custom_command(
      OUTPUT "${_awtk_modbus_assets_inc}"
      COMMAND ${Python3_EXECUTABLE}
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/update_res.py"
        all
        "--awtk_root=${AWTK_MODBUS_AWTK_SOURCE}"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMENT "Generating AWTK resources for modbus_server_awtk"
      VERBATIM
    )
    add_custom_target(awtk_modbus_update_res DEPENDS "${_awtk_modbus_assets_inc}")
  endif()

  if(EXISTS "${_awtk_modbus_assets_inc}" OR AWTK_MODBUS_AWTK_SOURCE)
    add_library(modbus_server_awtk_assets STATIC "${_awtk_modbus_demos_dir}/assets.c")
    target_link_libraries(modbus_server_awtk_assets PUBLIC modbus)
    if(TARGET awtk_modbus_update_res)
      add_dependencies(modbus_server_awtk_assets awtk_modbus_update_res)
    endif()

    add_executable(modbus_server_awtk
      "${_awtk_modbus_demos_dir}/server_awtk.c"
      "${_awtk_modbus_demos_dir}/modbus_memory_custom.c"
    )
    target_include_directories(modbus_server_awtk PRIVATE "${_awtk_modbus_demos_dir}")
    target_link_libraries(modbus_server_awtk PRIVATE modbus_server_awtk_assets modbus)
    set_target_properties(modbus_server_awtk PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${_awtk_modbus_demo_runtime_dir}"
      LINKER_LANGUAGE CXX
    )
    if(MSVC)
      target_link_libraries(modbus_server_awtk PRIVATE legacy_stdio_definitions)
      set_target_properties(modbus_server_awtk PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
    if(UNIX)
      set_target_properties(modbus_server_awtk PROPERTIES
        INSTALL_RPATH "${_modbus_install_rpath}"
        BUILD_WITH_INSTALL_RPATH FALSE
        MACOSX_RPATH ON
        BUILD_RPATH "$<TARGET_FILE_DIR:modbus>"
      )
    endif()
  else()
    message(STATUS "Skip modbus_server_awtk: res/assets.inc not found and AWTK_MODBUS_AWTK_SOURCE is unset")
  endif()
endif()

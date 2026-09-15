cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(serial_mux_console_mux LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/serial_mux_examples/console_mux)

include(${SdkRootDirPath}/CMakeLists.txt)

# Application glue only. The COBS core, shared-memory slots, hub and client
# roles come from the reusable component.serial_mux (selected in prj.conf).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/serial_mux_examples/console_mux/console_mux.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/serial_mux_examples/console_mux
             ${board_root}/${board}/serial_mux_examples/console_mux
             ${board_root}/${board}/serial_mux_examples/console_mux/${core_id}
)

mcux_add_macro(CC "-DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1")

include(${SdkRootDirPath}/${board_root}/${board}/serial_mux_examples/console_mux/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

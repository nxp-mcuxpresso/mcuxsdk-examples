cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(i3c_bus_slave LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/i3c_bus/slave)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES components/i3c_bus/example/slave/i3c_bus_slave.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/component_examples/i3c_bus/slave
             ${board_root}/${board}/component_examples/i3c_bus/slave/${core_id}
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x800"
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/i3c_bus/slave/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

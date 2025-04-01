
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(modbus_tcp_client LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/modbus_examples/tcp/modbus_client)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/modbus_examples/tcp/modbus_client/modbus_client.c
    SOURCES examples/modbus_examples/tcp/modbus_client/port.h
    SOURCES examples/modbus_examples/tcp/modbus_client/lwipopts.h
    SOURCES examples/modbus_examples/tcp/modbus_client/FreeRTOSConfig.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/modbus_examples/tcp/modbus_client
)

include(${SdkRootDirPath}/${board_root}/${board}/modbus_examples/tcp/modbus_client/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(modbus_tcp_server LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/modbus_examples/tcp/modbus_server)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/modbus_examples/tcp/modbus_server/modbus_server.c
    SOURCES examples/modbus_examples/tcp/modbus_server/portevent.c
    SOURCES examples/modbus_examples/tcp/modbus_server/port.h
    SOURCES examples/modbus_examples/tcp/modbus_server/porttcp.c
    SOURCES examples/modbus_examples/tcp/modbus_server/lwipopts.h
    SOURCES examples/modbus_examples/tcp/modbus_server/FreeRTOSConfig.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/modbus_examples/tcp/modbus_server
)

include(${SdkRootDirPath}/${board_root}/${board}/modbus_examples/tcp/modbus_server/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
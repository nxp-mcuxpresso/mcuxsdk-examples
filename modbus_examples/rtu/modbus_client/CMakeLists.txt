
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(modbus_rtu_client LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/modbus_examples/rtu/modbus_client)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/modbus_examples/rtu/modbus_client/modbus_client.c
    SOURCES examples/modbus_examples/rtu/modbus_client/modbus_serial.c
    SOURCES examples/modbus_examples/rtu/modbus_client/modbus_timer.c
    SOURCES examples/modbus_examples/rtu/modbus_client/portevent.c
    SOURCES examples/modbus_examples/rtu/modbus_client/port.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/modbus_examples/rtu/modbus_client
)

include(${SdkRootDirPath}/${board_root}/${board}/modbus_examples/rtu/modbus_client/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
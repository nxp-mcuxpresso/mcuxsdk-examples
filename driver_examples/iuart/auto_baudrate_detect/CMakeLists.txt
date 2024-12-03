cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(iuart_auto_baudrate_detect LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/uart/auto_baudrate_detect)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/iuart/auto_baudrate_detect/uart_auto_baudrate_detect.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/uart/auto_baudrate_detect
             examples/_boards/${board}/driver_examples/uart/auto_baudrate_detect/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/uart/auto_baudrate_detect/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
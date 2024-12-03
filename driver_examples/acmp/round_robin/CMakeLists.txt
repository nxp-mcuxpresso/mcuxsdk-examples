cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(acmp_round_robin LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/acmp/round_robin)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/acmp/round_robin/acmp_round_robin.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/acmp/round_robin
             examples/_boards/${board}/driver_examples/acmp/round_robin/${core_id}
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=256"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/acmp/round_robin/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
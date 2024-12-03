cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usart_transfer_sync_b2b_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/usart/transfer_sync_b2b/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_miniusart/transfer_sync_b2b/master/usart_transfer_sync_master.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/usart/transfer_sync_b2b/master
             examples/_boards/${board}/driver_examples/usart/transfer_sync_b2b/master/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/usart/transfer_sync_b2b/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
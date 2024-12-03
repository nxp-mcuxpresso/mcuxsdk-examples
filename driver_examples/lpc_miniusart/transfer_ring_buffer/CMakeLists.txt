cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(usart_transfer_ring_buffer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/usart/transfer_ring_buffer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_miniusart/transfer_ring_buffer/usart_transfer_ring_buffer.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/usart/transfer_ring_buffer
             examples/_boards/${board}/driver_examples/usart/transfer_ring_buffer/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/usart/transfer_ring_buffer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
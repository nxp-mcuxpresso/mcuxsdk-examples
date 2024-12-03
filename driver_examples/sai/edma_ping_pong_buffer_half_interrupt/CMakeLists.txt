cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sai_edma_ping_pong_buffer_half_interrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer_half_interrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/edma_ping_pong_buffer_half_interrupt/sai_edma_ping_pong_buffer_half_interrupt.c
            examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/sai
             examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer_half_interrupt
             examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer_half_interrupt/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer_half_interrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
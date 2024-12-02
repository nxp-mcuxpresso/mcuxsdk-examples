cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sai_edma_ping_pong_buffer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/edma_ping_pong_buffer/sai_edma_ping_pong_buffer.c
            examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer
             examples/driver_examples/sai
             examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/sai/edma_ping_pong_buffer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
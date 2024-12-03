cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(i2s_dma_transfer_tfa9xxx LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9xxx)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcomm/i2s/dma_transfer_tfa9xxx/i2s_dma_transfer_tfa9xxx.c
            examples/driver_examples/flexcomm/i2s/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/flexcomm/i2s
             examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9xxx
             examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9xxx/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9xxx/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
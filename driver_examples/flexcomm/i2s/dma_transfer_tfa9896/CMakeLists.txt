cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(i2s_dma_transfer_tfa9896 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9896)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcomm/i2s/dma_transfer_tfa9896/i2s_dma_transfer_tfa9896.c
            examples/driver_examples/flexcomm/i2s/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/flexcomm/i2s
             examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9896
             examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9896/${core_id}
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x800"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x800"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x800\""
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/i2s/dma_transfer_tfa9896/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
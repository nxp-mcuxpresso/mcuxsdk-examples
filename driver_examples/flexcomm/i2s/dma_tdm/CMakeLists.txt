cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(i2s_dma_tdm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/i2s/dma_tdm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcomm/i2s/dma_tdm/i2s_dma_tdm.c
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/i2s/dma_tdm
             ${board_root}/${board}
             ${board_root}/${board}/driver_examples/i2s/dma_tdm/${core_id}
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/i2s/dma_tdm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sdio_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/sdmmc_examples/sdio_freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdio_freertos/sdio_freertos.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/sdmmc/example/sdio_freertos
)

include(${SdkRootDirPath}/${board_root}/${board}/sdmmc_examples/sdio_freertos/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/sdmmc_examples/sdio_freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

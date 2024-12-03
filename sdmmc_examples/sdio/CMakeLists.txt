
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sdio LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/sdmmc_examples/sdio)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdio/sdio.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/sdmmc/example/sdio
)

include(${SdkRootDirPath}/examples/_boards/${board}/sdmmc_examples/sdio/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/sdmmc_examples/sdio/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)


cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sdcard_fatfs LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/sdmmc_examples/sdcard_fatfs)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdcard_fatfs/sdcard_fatfs.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/sdmmc/example/sdcard_fatfs
)

include(${SdkRootDirPath}/examples/_boards/${board}/sdmmc_examples/sdcard_fatfs/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/sdmmc_examples/sdcard_fatfs/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

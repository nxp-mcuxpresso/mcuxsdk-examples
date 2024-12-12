
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sdcard_fatfs LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/sdmmc_examples/sdcard_fatfs)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sdmmc/example/sdcard_fatfs/sdcard_fatfs.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/sdmmc/example/sdcard_fatfs
)

mcux_add_linker_symbol(
      SYMBOLS "__stack_size__=0x1000 __heap_size__=0x1000"
)

include(${SdkRootDirPath}/${board_root}/${board}/sdmmc_examples/sdcard_fatfs/${core_id}/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/sdmmc_examples/sdcard_fatfs/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

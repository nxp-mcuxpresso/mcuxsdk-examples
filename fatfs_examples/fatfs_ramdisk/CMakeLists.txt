cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fatfs_ramdisk LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/fatfs_examples/fatfs_ramdisk)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES fatfs_ramdisk.c
)

mcux_add_include(
    INCLUDES .
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

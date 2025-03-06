cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(xbic LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/xbic)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES xbic.c
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/xbic/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
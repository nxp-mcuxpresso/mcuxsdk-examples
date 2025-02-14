cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(swt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/swt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES swt.c
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/swt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
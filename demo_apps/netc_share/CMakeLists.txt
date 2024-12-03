cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(netc_share LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/netc_share)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
    examples/demo_apps/netc_share/netc_share.c
    examples/demo_apps/netc_share/FreeRTOSConfig.h
    examples/demo_apps/netc_share/lwipopts.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
    examples/demo_apps/netc_share
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/netc_share/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

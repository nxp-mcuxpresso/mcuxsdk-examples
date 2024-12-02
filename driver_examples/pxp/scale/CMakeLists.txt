cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pxp_scale LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/pxp/scale)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/pxp/scale/pxp_scale.c
            examples/_boards/${board}/display_support.h
            examples/_boards/${board}/display_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/pxp/scale
             examples/_boards/${board}
             examples/_boards/${board}/driver_examples/pxp/scale/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/pxp/scale/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
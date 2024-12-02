cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hello_world_swo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/hello_world_swo)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/hello_world_swo/hello_world_swo.c
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=0\
       -DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0"
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/hello_world_swo/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
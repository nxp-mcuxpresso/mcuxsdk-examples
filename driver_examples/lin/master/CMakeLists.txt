cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lin_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lin/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lin/master/lin_master.c
            examples/_boards/${board}/driver_examples/lin/master/lin_cfg.h
            examples/_boards/${board}/driver_examples/lin/master/lin_cfg.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lin/master
             examples/_boards/${board}/driver_examples/lin/master/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lin/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
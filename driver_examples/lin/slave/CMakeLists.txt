cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lin_slave LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lin/slave)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lin/slave/lin_slave.c
            examples/_boards/${board}/driver_examples/lin/slave/lin_cfg.h
            examples/_boards/${board}/driver_examples/lin/slave/lin_cfg.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lin/slave
             examples/_boards/${board}/driver_examples/lin/slave/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lin/slave/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
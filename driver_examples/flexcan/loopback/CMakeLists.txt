cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexcan_loopback LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/flexcan/loopback)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcan/loopback/flexcan_loopback.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/flexcan/loopback
             examples/_boards/${board}/driver_examples/flexcan/loopback/${core_id}
)

mcux_add_macro(
    CC "-DFLEXCAN_WAIT_TIMEOUT=1000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/flexcan/loopback/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
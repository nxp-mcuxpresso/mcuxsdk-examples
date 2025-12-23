cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexcan_remote_request_non_blocking LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/flexcan/remote_request_non_blocking)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcan/remote_request_non_blocking/flexcan_remote_request_non_blocking.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/flexcan/remote_request_non_blocking
             ${board_root}/${board}/driver_examples/flexcan/remote_request_non_blocking/${core_id}
)

mcux_add_macro(
    CC "-DFLEXCAN_WAIT_TIMEOUT=1000"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/flexcan/remote_request_non_blocking/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexcan_pretended_networking_wakeup LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexcan/pretended_networking_wakeup/flexcan_pretended_networking_wakeup.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup
             ${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup/${core_id}
)

mcux_add_macro(
    CC "-DFLEXCAN_WAIT_TIMEOUT=1000\
       -DPRINTF_ADVANCED_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
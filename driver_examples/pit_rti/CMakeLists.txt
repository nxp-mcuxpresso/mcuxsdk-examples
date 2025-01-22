cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pit_rti LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/pit_rti)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES pit_rti.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/pit_rti
             ${board_root}/${board}/driver_examples/pit_rti/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/pit_rti/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
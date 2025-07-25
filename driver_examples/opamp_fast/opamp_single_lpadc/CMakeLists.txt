cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(opamp_single_lpadc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/opamp/opamp_lpadc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/opamp_fast/opamp_single_lpadc/opamp_lpadc.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/opamp/opamp_lpadc
             ${board_root}/${board}/driver_examples/opamp/opamp_lpadc/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/opamp/opamp_lpadc/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
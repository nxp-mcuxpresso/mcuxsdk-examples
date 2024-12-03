cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpadc_dual_single_ended_conversion LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lpadc/dual_single_ended_conversion)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpadc/dual_single_ended_conversion/lpadc_dual_single_ended_conversion.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lpadc/dual_single_ended_conversion
             examples/_boards/${board}/driver_examples/lpadc/dual_single_ended_conversion/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lpadc/dual_single_ended_conversion/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
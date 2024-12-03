cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cadc_parallel_simultaneous_conversion LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/cadc/parallel_simultaneous_conversion)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dsc_cadc/parallel_simultaneous_conversion/cadc_parallel_simultaneous_conversion.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/cadc/parallel_simultaneous_conversion
             examples/_boards/${board}/driver_examples/cadc/parallel_simultaneous_conversion/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/cadc/parallel_simultaneous_conversion/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
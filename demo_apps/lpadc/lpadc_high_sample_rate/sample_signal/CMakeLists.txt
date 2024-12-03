cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpadc_high_sample_rate_sample_signal LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/lpadc_high_sample_rate/sample_signal)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/lpadc/lpadc_high_sample_rate/sample_signal/lpadc_high_sample_rate_sample_signal.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/lpadc_high_sample_rate/sample_signal/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
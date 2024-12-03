
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dpu_1_figure_display LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/dpu/figure_display)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    SOURCES dpu_figure_display.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/dpu/figure_display/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lcdifv3_figure_display LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lcdifv3/figure_display)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lcdifv3/figure_display/figure_display.c
            examples/_boards/${board}/driver_examples/lcdifv3/lcdifv3_support.c
            examples/_boards/${board}/driver_examples/lcdifv3/lcdifv3_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lcdifv3
             examples/_boards/${board}/driver_examples/lcdifv3/figure_display/${core_id}
             examples/_boards/${board}/driver_examples/lcdifv3/figure_display
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lcdifv3/figure_display/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
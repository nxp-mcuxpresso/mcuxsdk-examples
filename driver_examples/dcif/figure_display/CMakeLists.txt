cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dcif_figure_display LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/dcif/figure_display)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dcif/figure_display/dcif_figure_display.c
            ${board_root}/${board}/driver_examples/dcif/pin_mux.c
            ${board_root}/${board}/driver_examples/dcif/pin_mux.h
            ${board_root}/${board}/driver_examples/dcif/${multicore_foldername}/hardware_init.c
            ${board_root}/${board}/driver_examples/dcif/${multicore_foldername}/app.h
            ${board_root}/${board}/driver_examples/dcif/dcif_support.c
            ${board_root}/${board}/driver_examples/dcif/dcif_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/dcif
             ${board_root}/${board}/driver_examples/dcif/${multicore_foldername}
             ${board_root}/${board}/driver_examples/dcif/figure_display/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/dcif/figure_display/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

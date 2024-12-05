cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lcdif_rotate_flip LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/lcdif/rotate_flip)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lcdif/rotate_flip/lcdif_rotate_flip.c
            ${board_root}/${board}/driver_examples/lcdif/lcdif_support.h
            ${board_root}/${board}/driver_examples/lcdif/lcdif_support.c
            ${board_root}/${board}/driver_examples/lcdif/pin_mux.c
            ${board_root}/${board}/driver_examples/lcdif/pin_mux.h
            ${board_root}/${board}/driver_examples/lcdif/${multicore_foldername}/hardware_init.c
            ${board_root}/${board}/driver_examples/lcdif/${multicore_foldername}/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lcdif
             ${board_root}/${board}/driver_examples/lcdif/${multicore_foldername}
             ${board_root}/${board}/driver_examples/lcdif/rotate_flip/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/lcdif/rotate_flip/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
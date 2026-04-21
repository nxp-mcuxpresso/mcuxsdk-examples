
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(h264_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/display_examples/h264_freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/h264_freertos/h264_freertos.c
            examples/display_examples/h264_freertos/display.c
            examples/display_examples/h264_freertos/display.h
            examples/display_examples/h264_freertos/h264_dec.h
            examples/display_examples/h264_freertos/h264_dec.cpp
            examples/display_examples/h264_freertos/sdcard.c
            examples/display_examples/h264_freertos/sdcard.h
            examples/display_examples/h264_freertos/FreeRTOSConfig.h
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/display_examples/h264_freertos
             ${board_root}/${board}/display_examples/h264_freertos/${multicore_foldername}
             examples/display_examples/h264_freertos/
)


# Add additional configuration
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x1800000"
    CX "--no_exceptions"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib --predefine=\"-D__stack_size__=0x1000\" --predefine=\"-D__heap_size__=0x1800000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x1800000"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-DSDK_USE_SYSCALL_STUB=0"
)

include(${SdkRootDirPath}/${board_root}/${board}/display_examples/h264_freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

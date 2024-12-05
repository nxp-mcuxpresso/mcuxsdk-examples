
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sd_png LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/display_examples/sd_png)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/sd_png/sd_png.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/sdmmc_config.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/display_examples/sd_png
             ${board_root}/${board}/display_examples/sd_png/${multicore_foldername}
             examples/display_examples/sd_png
)

mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x20000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\" --predefine=\"-D__heap_size__=0x20000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x20000"
)

include(${SdkRootDirPath}/${board_root}/${board}/display_examples/sd_png/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
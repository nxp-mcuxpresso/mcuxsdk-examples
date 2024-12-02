
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sd_jpeg LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/display_examples/sd_jpeg)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/sd_jpeg/sd_jpeg.c
            examples/display_examples/sd_jpeg/jconfig.h
            examples/display_examples/sd_jpeg/ffconf.h
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/sdmmc_config.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/display_examples/sd_jpeg
             examples/_boards/${board}/display_examples/sd_jpeg/${multicore_foldername}
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

include(${SdkRootDirPath}/examples/_boards/${board}/display_examples/sd_jpeg/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
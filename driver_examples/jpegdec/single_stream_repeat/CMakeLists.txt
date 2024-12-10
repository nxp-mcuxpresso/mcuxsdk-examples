
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(jpegdec_single_stream_repeat LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/jpegdec/single_stream_repeat)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/jpegdec/single_stream_repeat/single_stream_repeat.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/driver_examples/jpegdec/pin_mux.c
            ${board_root}/${board}/driver_examples/jpegdec/pin_mux.h
            ${board_root}/${board}/driver_examples/jpegdec/${multicore_foldername}/hardware_init.c
            ${board_root}/${board}/driver_examples/jpegdec/${multicore_foldername}/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/driver_examples/jpegdec
             ${board_root}/${board}/driver_examples/jpegdec/${multicore_foldername}
             examples/driver_examples/jpegdec/single_stream_repeat
)

mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
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

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/jpegdec/single_stream_repeat/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
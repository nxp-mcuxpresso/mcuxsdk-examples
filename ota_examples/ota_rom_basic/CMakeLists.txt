
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ota_rom_basic LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ota_examples/ota_rom_basic)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/ota_rom_basic/ota_rom_basic.c
            examples/ota_examples/ota_rom_basic/xmodem.c
            examples/ota_examples/ota_rom_basic/xmodem.h
            ${board_root}/${board}/ota_examples/ota_rom_basic/platform_bindings.c
            ${board_root}/${board}/ota_examples/ota_rom_basic/platform_bindings.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/ota_rom_basic
             ${board_root}/${board}
             examples/ota_examples/ota_rom_basic
)

# Add additional configuration
mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSHELL_PRINT_COPYRIGHT=0"
)
mcux_add_iar_configuration(
    CC "--diag_suppress=Pe257"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-O1"
)

# Stack and heap setup
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x400"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x400\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x400"
)

include(${SdkRootDirPath}/${board_root}/${board}/ota_examples/ota_rom_basic/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

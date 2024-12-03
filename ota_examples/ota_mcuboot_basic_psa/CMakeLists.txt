
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ota_mcuboot_basic_psa LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ota_examples/ota_mcuboot_basic_psa)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ota_examples/ota_mcuboot_basic_psa/ota_mcuboot_basic_psa.c
            examples/ota_examples/ota_mcuboot_basic_psa/error.h
            examples/ota_examples/ota_mcuboot_basic_psa/update.h
            examples/ota_examples/ota_mcuboot_basic_psa/update.c
            examples/ota_examples/ota_mcuboot_basic/xmodem.c
            examples/ota_examples/ota_mcuboot_basic/xmodem.h
            examples/_boards/${board}/ota_examples/ota_mcuboot_basic_psa/platform_bindings.c
            examples/_boards/${board}/ota_examples/ota_mcuboot_basic_psa/platform_bindings.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/_boards/${board}/ota_examples/ota_mcuboot_basic
             examples/_boards/${board}/ota_examples/ota_mcuboot_basic_psa
             examples/_boards/${board}/ota_examples/mcuboot_opensource
             examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning
             examples/ota_examples/ota_mcuboot_basic
             examples/ota_examples/ota_mcuboot_basic_psa
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.c
            examples/_boards/${board}/ota_examples/mcuboot_opensource/sblconfig.h
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

include(${SdkRootDirPath}/middleware/mcuboot_opensource/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/ota_mcuboot_basic/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

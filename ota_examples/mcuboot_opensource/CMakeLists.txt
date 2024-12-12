cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mcuboot_opensource LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ota_examples/mcuboot_opensource)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ota_examples/mcuboot_opensource
    INCLUDES
        .
        flash_partitioning
        ${multicore_foldername}
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ota_examples/mcuboot_opensource
    PROJECT_BASE_PATH source
    SOURCES
        main.c
        sblconfig.h
        flash_partitioning/flash_partitioning.h
        flash_partitioning/flash_partitioning.c
)


# Add additional configuration
mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSHELL_PRINT_COPYRIGHT=0"

)
mcux_add_iar_configuration(
    CC "--diag_suppress=Pe257,Pa039"
)

# MCUboot relies on this option otherwise the build fails under IAR
mcux_remove_iar_configuration(
    CC "--use_c++_inline"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release flash_release release
    CC "-Oz"
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release flash_release release
    CC "-Os"
)

include(${SdkRootDirPath}/examples/_boards/${board}/ota_examples/mcuboot_opensource/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

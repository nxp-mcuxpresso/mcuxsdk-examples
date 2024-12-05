cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(trdc_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/trdc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/trdc/multicore/trdc_core1.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/trdc/${core_id}
)

mcux_add_macro(
    CC "-DMCMGR_HANDLE_EXCEPTIONS=0\
       -D__SEMIHOST_HARDFAULT_DISABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/trdc/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(netc_psi_vsi_message_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/netc/psi_vsi_message/)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/netc/psi_vsi_message/netc_psi_vsi_message_core1.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/netc/psi_vsi_message/${core_id}
)

mcux_add_macro(
    CC "-DMCMGR_HANDLE_EXCEPTIONS=1\
       -D__SEMIHOST_HARDFAULT_DISABLE=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__multicore__=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/netc/psi_vsi_message//reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

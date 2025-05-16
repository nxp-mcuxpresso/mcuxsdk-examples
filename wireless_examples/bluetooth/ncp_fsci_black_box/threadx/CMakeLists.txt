cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ncp_fsci_black_box LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH middleware/wireless/ble_controller/src/KW4x
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_iar_configuration(
    TARGETS  debug
    CC "     --debug "
)
mcux_add_iar_configuration(
    TARGETS  debug
    LD "     --semihosting "
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
           bluetooth/examples/ncp_fsci_black_box/ncp_fsci_black_box.c
           bluetooth/examples/ncp_fsci_black_box/ncp_fsci_black_box.h
           ../../examples/wireless_examples/bluetooth/ncp_fsci_black_box/readme.md
           ../../examples/_boards/${board}/wireless_examples/bluetooth/ncp_fscibb/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES bluetooth/examples/ncp_fsci_black_box
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/bluetooth/ncp_fscibb/app_preinclude.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/ncp_fscibb/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/ncp_fscibb.bin
)
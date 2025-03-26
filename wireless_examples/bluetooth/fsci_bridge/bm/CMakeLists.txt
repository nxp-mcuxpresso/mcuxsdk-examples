cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fsci_bridge_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/bluetooth/fsci_bridge/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
        bluetooth/application/common/app_conn.h
        bluetooth/application/common/ble_init.h
        bluetooth/application/common/app_conn.h
        framework/services/FSCI/Interface/FsciInterface.h
        framework/services/FSCI/Source/FsciCommunication.c
        framework/services/FSCI/Source/FsciCommunication.h
        framework/services/FSCI/Source/FsciCommands.c
        framework/services/FSCI/Source/FsciCommands.h
        framework/services/NVM/Source/NV_FsciCommands.c
        framework/services/NVM/Source/NV_FsciCommands.h
        bluetooth/examples/fsci_bridge/fsci_bridge.h
        bluetooth/examples/fsci_bridge/fsci_bridge.c
        bluetooth/examples/fsci_bridge/main.c
        ../../examples/wireless_examples/bluetooth/fsci_bridge/readme.md
        ../../examples/_boards/${board}/wireless_examples/bluetooth/fsci_bridge/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/application/common
        bluetooth/examples/fsci_bridge/
        framework/services/FSCI/Interface
        framework/services/FSCI/Source
        bluetooth/host/interface
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/bluetooth/fsci_bridge/app_preinclude.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/fsci_bridge/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/fsci_bridge_${board}.srec
)

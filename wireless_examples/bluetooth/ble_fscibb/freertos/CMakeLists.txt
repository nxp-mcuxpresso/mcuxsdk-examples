cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ble_fsci_black_box_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
        ${board_root}/${board}/wireless_examples/bluetooth/ble_fscibb/${core_id}/freertos
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
           ../../examples/wireless_examples/bluetooth/common/freertos/main.c
           ../../examples/wireless_examples/bluetooth/common/freertos/FreeRTOSConfig.h
           ../../examples/wireless_examples/bluetooth/common/app_conn.c
           ../../examples/wireless_examples/bluetooth/common/app_conn.h
           ../../examples/wireless_examples/bluetooth/common/ble_init.c
           ../../examples/wireless_examples/bluetooth/common/ble_init.h
           ../../examples/wireless_examples/bluetooth/common/app_conn.c
           ../../examples/wireless_examples/bluetooth/common/app_conn.h
           ../../examples/wireless_examples/bluetooth/common/app_nvm.c
           ../../examples/wireless_examples/bluetooth/common/app_low_power.c
           ../../examples/wireless_examples/bluetooth/common/ble_init.c
           ../../examples/wireless_examples/bluetooth/common/ble_init.h
           bluetooth/examples/ble_fscibb/ble_fsci_black_box.c
           bluetooth/examples/ble_fscibb/ble_fsci_black_box.h
           bluetooth/examples/ble_fscibb/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/ble_fscibb/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/ble_fscibb/
        ../../examples/wireless_examples/bluetooth/common/freertos
        ../../examples/wireless_examples/bluetooth/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/ble_fscibb/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/ble_fsci_black_box_${board}.srec
)

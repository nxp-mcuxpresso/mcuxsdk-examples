cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(adv_ext_central_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/adv_ext_central/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
        ../../examples/wireless_examples/bluetooth/common/bm/main.c
        ../../examples/wireless_examples/bluetooth/common/app_conn.c
        ../../examples/wireless_examples/bluetooth/common/app_conn.h
        ../../examples/wireless_examples/bluetooth/common/ble_init.c
        ../../examples/wireless_examples/bluetooth/common/ble_init.h
        ../../examples/wireless_examples/bluetooth/common/app_connection.c
        ../../examples/wireless_examples/bluetooth/common/app_scanner.c
        ../../examples/wireless_examples/bluetooth/common/app_scanner.h
        ../../examples/wireless_examples/bluetooth/common/app_nvm.c
        bluetooth/examples/adv_ext_central/app_config.c
        bluetooth/examples/adv_ext_central/gatt_db.h
        bluetooth/examples/adv_ext_central/gatt_uuid128.h
        bluetooth/examples/adv_ext_central/adv_ext_central.c
        bluetooth/examples/adv_ext_central/adv_ext_central.h
        bluetooth/examples/adv_ext_central/readme.md
        ../../${board_root}/${board}/wireless_examples/bluetooth/adv_ext_central/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/adv_ext_central/
        ../../examples/wireless_examples/bluetooth/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/adv_ext_central/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/adv_ext_central_${board}.srec
)

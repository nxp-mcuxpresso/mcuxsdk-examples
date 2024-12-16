cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(adv_ext_peripheral_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
        ${board_root}/${board}/wireless_examples/bluetooth/adv_ext_peripheral/${core_id}/freertos
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
           ../../examples/wireless_examples/bluetooth/common/app_connection.c
           ../../examples/wireless_examples/bluetooth/common/app_advertiser.c
           ../../examples/wireless_examples/bluetooth/common/app_advertiser.h
           ../../examples/wireless_examples/bluetooth/common/app_nvm.c
           ../../examples/wireless_examples/bluetooth/common/app_low_power.c
           bluetooth/examples/adv_ext_peripheral/app_config.c
           bluetooth/examples/adv_ext_peripheral/gatt_db.h
           bluetooth/examples/adv_ext_peripheral/gatt_uuid128.h
           bluetooth/examples/adv_ext_peripheral/adv_ext_peripheral.c
           bluetooth/examples/adv_ext_peripheral/adv_ext_peripheral.h
           bluetooth/examples/adv_ext_peripheral/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/adv_ext_peripheral/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/adv_ext_peripheral/
        ../../examples/wireless_examples/bluetooth/common/freertos
        ../../examples/wireless_examples/bluetooth/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/adv_ext_peripheral/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/adv_ext_peripheral_${board}.srec
)

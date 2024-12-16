cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(otap_client_att_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
	${board_root}/${board}/wireless_examples/bluetooth/otac_att/${core_id}/freertos
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
           ../../examples/wireless_examples/bluetooth/common/app_advertiser.c
           ../../examples/wireless_examples/bluetooth/common/app_advertiser.h
           ../../examples/wireless_examples/bluetooth/common/app_connection.c
           ../../examples/wireless_examples/bluetooth/common/app_nvm.c
           ../../examples/wireless_examples/bluetooth/common/app_low_power.c
           ../../examples/wireless_examples/bluetooth/common/otap_client/otap_client.c
           ../../examples/wireless_examples/bluetooth/common/otap_client/otap_client.h
           bluetooth/examples/otac_att/app_config.c
           bluetooth/examples/otac_att/gatt_db.h
           bluetooth/examples/otac_att/gatt_uuid128.h
           bluetooth/examples/otac_att/otap_client_att.c
           bluetooth/examples/otac_att/otap_client_att.h
           bluetooth/examples/otac_att/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/otac_att/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/otac_att/
        ../../examples/wireless_examples/bluetooth/common/freertos
        ../../examples/wireless_examples/bluetooth/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/otac_att/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_add_linker_symbol(SYMBOLS "gUseInternalStorageLink_d=1")

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/otap_client_att_${board}.srec
)

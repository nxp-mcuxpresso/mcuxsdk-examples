cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(otap_client_l2cap_credit_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
    ${board_root}/${board}/wireless_examples/bluetooth/otac_l2cap/${core_id}/bm
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
           ../../examples/wireless_examples/bluetooth/common/app_advertiser.c
           ../../examples/wireless_examples/bluetooth/common/app_advertiser.h
           ../../examples/wireless_examples/bluetooth/common/app_connection.c
           ../../examples/wireless_examples/bluetooth/common/app_nvm.c
           ../../examples/wireless_examples/bluetooth/common/otap_client/otap_client.c
           ../../examples/wireless_examples/bluetooth/common/otap_client/otap_client.h
           bluetooth/examples/otac_l2cap/app_config.c
           bluetooth/examples/otac_l2cap/gatt_db.h
           bluetooth/examples/otac_l2cap/gatt_uuid128.h
           bluetooth/examples/otac_l2cap/otap_client_l2cap_credit.c
           bluetooth/examples/otac_l2cap/otap_client_l2cap_credit.h
           bluetooth/examples/otac_l2cap/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/otac_l2cap/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/otac_l2cap/
        ../../examples/wireless_examples/bluetooth/common/
        ../../examples/wireless_examples/bluetooth/common/otap_client
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/otac_l2cap/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_add_linker_symbol(SYMBOLS "gUseInternalStorageLink_d=1")

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/otap_client_l2cap_${board}.srec
)

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(otap_client_l2cap_credit_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/_boards/${board}/wireless_examples/bluetooth/
    examples/_boards/${board}/wireless_examples/bluetooth/otac_l2cap/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    SOURCES
           ../../../../wireless_examples/bluetooth/common/bm/main.c
           ../../../../wireless_examples/bluetooth/common/app_conn.c
           ../../../../wireless_examples/bluetooth/common/app_conn.h
           ../../../../wireless_examples/bluetooth/common/ble_init.c
           ../../../../wireless_examples/bluetooth/common/ble_init.h
           ../../../../wireless_examples/bluetooth/common/app_advertiser.c
           ../../../../wireless_examples/bluetooth/common/app_advertiser.h
           ../../../../wireless_examples/bluetooth/common/app_connection.c
           ../../../../wireless_examples/bluetooth/common/app_nvm.c
           ../../../../wireless_examples/bluetooth/common/otap_client/otap_client.c
           ../../../../wireless_examples/bluetooth/common/otap_client/otap_client.h
           ../app_config.c
           ../gatt_db.h
           ../gatt_uuid128.h
           ../otap_client_l2cap_credit.c
           ../otap_client_l2cap_credit.h
           ../../../../_boards/${board}/wireless_examples/bluetooth/otac_l2cap/example_board_readme.md
           ../readme.md
)

mcux_add_include(
    INCLUDES .
             ../
             ../../common
             ../../../../wireless_examples/bluetooth/common/otap_client
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/otac_l2cap/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_add_linker_symbol(SYMBOLS "gUseInternalStorageLink_d=1")

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/otap_client_l2cap_${board}.srec
)

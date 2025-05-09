cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(otap_client_att_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
    ${board_root}/${board}/wireless_examples/bluetooth/otac_att/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
           bluetooth/application/common/bm/main.c
           bluetooth/application/common/app_conn.c
           bluetooth/application/common/app_conn.h
           bluetooth/application/common/ble_init.c
           bluetooth/application/common/ble_init.h
           bluetooth/application/common/app_advertiser.c
           bluetooth/application/common/app_advertiser.h
           bluetooth/application/common/app_connection.c
           bluetooth/application/common/app_nvm.c
           bluetooth/application/common/otap_client/otap_client.c
           bluetooth/application/common/otap_client/otap_client.h
           bluetooth/examples/otac_att/app_config.c
           bluetooth/examples/otac_att/gatt_db.h
           bluetooth/examples/otac_att/gatt_uuid128.h
           bluetooth/examples/otac_att/otap_client_att.c
           bluetooth/examples/otac_att/otap_client_att.h
           ../../examples/wireless_examples/bluetooth/otac_att/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/otac_att/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/otac_att/
        bluetooth/application/common/
        bluetooth/application/common/otap_client
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/bluetooth/otac_att/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/otac_att/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_add_linker_symbol(SYMBOLS "gUseInternalStorageLink_d=1")

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/otap_client_att_${board}.srec
)

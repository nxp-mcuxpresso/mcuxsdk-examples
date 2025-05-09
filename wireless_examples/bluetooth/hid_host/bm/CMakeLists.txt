cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hid_host_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/hid_host/${core_id}/bm
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
           bluetooth/application/common/app_connection.c
           bluetooth/application/common/app_scanner.c
           bluetooth/application/common/app_scanner.h
           bluetooth/application/common/app_nvm.c
           bluetooth/examples/hid_host/app_config.c
           bluetooth/examples/hid_host/gatt_db.h
           bluetooth/examples/hid_host/gatt_uuid128.h
           bluetooth/examples/hid_host/hid_host.c
           bluetooth/examples/hid_host/hid_host.h
           ../../examples/wireless_examples/bluetooth/hid_host/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/hid_host/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/hid_host/
        bluetooth/application/common/
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/bluetooth/hid_host/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/hid_host/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/hid_host_${board}.srec
)

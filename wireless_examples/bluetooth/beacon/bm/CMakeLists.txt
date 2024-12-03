cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(beacon_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/_boards/${board}/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/bluetooth/beacon/${core_id}/bm
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
           ../../../../wireless_examples/bluetooth/common/app_nvm.c
           ../app_config.c
           ../gatt_db.h
           ../gatt_uuid128.h
           ../beacon.c
           ../beacon.h
           ../../../../_boards/${board}/wireless_examples/bluetooth/beacon/example_board_readme.md
           ../readme.md
)

mcux_add_include(
    INCLUDES .
             ../
             ../../common
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/beacon/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/beacon_${board}.srec
)

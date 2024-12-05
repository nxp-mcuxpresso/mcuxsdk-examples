cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(eatt_central_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/eatt_central/${core_id}/bm
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
           ../../../../wireless_examples/bluetooth/common/app_connection.c
           ../../../../wireless_examples/bluetooth/common/app_scanner.c
           ../../../../wireless_examples/bluetooth/common/app_scanner.h
           ../../../../wireless_examples/bluetooth/common/app_nvm.c
           ../app_config.c
           ../gatt_db.h
           ../gatt_uuid128.h
           ../eatt_central.c
           ../eatt_central.h
           ../../../../../${board_root}/${board}/wireless_examples/bluetooth/eatt_central/example_board_readme.md
           ../readme.md
)

mcux_add_include(
    INCLUDES .
             ../
             ../../common
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/eatt_central/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/eatt_central_${board}.srec
)

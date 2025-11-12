cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(loc_reader_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/loc_reader/
        ${board_root}/${board}/wireless_examples/bluetooth/loc_reader/${core_id}/bm
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
           bluetooth/application/common/app_scanner.c
           bluetooth/application/common/app_scanner.h
           bluetooth/application/common/app_nvm.c
           bluetooth/examples/loc_reader/app_config.c
           bluetooth/examples/loc_reader/gatt_db.h
           bluetooth/examples/loc_reader/gatt_uuid128.h
           bluetooth/examples/loc_reader/loc_reader.c
           bluetooth/examples/loc_reader/loc_reader.h
           bluetooth/examples/loc_reader/shell_loc_reader.c
           bluetooth/examples/loc_reader/shell_loc_reader.h
           ../../examples/wireless_examples/bluetooth/loc_reader/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/loc_reader/example_board_readme.md
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES 
        bluetooth/application/common
        bluetooth/examples/loc_reader
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/bluetooth/loc_reader/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/loc_reader/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/loc_reader_${board}.srec
)

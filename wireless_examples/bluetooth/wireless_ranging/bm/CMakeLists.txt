cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wireless_ranging_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/wireless_ranging/
        ${board_root}/${board}/wireless_examples/bluetooth/wireless_ranging/${core_id}/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_macro(
    SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY=1
)

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
           bluetooth/examples/wireless_ranging/app_config.c
           bluetooth/examples/wireless_ranging/gatt_db.h
           bluetooth/examples/wireless_ranging/gatt_uuid128.h
           bluetooth/examples/wireless_ranging/wireless_ranging.c
           bluetooth/examples/wireless_ranging/wireless_ranging.h
           bluetooth/examples/wireless_ranging/wireless_ranging_service.c
           bluetooth/examples/wireless_ranging/wireless_ranging_service.h
           bluetooth/examples/wireless_ranging/wireless_ranging_measurement.c
           bluetooth/examples/wireless_ranging/wireless_ranging_measurement.h
           bluetooth/examples/wireless_ranging/wireless_ranging_globals.c
           bluetooth/examples/wireless_ranging/wireless_ranging_cli.c
           bluetooth/examples/wireless_ranging/wireless_ranging_cli.h
           bluetooth/examples/wireless_ranging/cmdparse.c
           bluetooth/examples/wireless_ranging/cmdparse.h
           bluetooth/examples/wireless_ranging/app_version.c
           bluetooth/examples/wireless_ranging/app_version.h
           bluetooth/examples/wireless_ranging/app_version_definitions.h
           ../../examples/wireless_examples/bluetooth/wireless_ranging/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/wireless_ranging/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES 
        bluetooth/application/common
        bluetooth/examples/wireless_ranging
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/bluetooth/wireless_ranging/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/wireless_ranging/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/wireless_ranging_bm.bin
)

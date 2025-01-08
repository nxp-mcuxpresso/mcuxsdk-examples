cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(digital_key_car_anchor_bm LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples 
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
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
           bluetooth/examples/digital_key_car_anchor/app_config.c
           bluetooth/examples/digital_key_car_anchor/gatt_db.h
           bluetooth/examples/digital_key_car_anchor/gatt_uuid128.h
           bluetooth/examples/digital_key_car_anchor/app_digital_key_car_anchor.c
           bluetooth/examples/digital_key_car_anchor/app_digital_key_car_anchor.h
           bluetooth/examples/digital_key_car_anchor/digital_key_car_anchor.c
           bluetooth/examples/digital_key_car_anchor/digital_key_car_anchor.h
           bluetooth/examples/digital_key_car_anchor/shell_digital_key_car_anchor.c
           bluetooth/examples/digital_key_car_anchor/shell_digital_key_car_anchor.h
           bluetooth/examples/digital_key_car_anchor/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/digital_key_car_anchor/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/digital_key_car_anchor/
        bluetooth/application/common/
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/bluetooth/digital_key_car_anchor/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/digital_key_car_anchor/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/digital_key_car_anchor_${board}.srec
)

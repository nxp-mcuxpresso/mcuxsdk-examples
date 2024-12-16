cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wireless_uart_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/w_uart/${core_id}/bm
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
           ../../examples/wireless_examples/bluetooth/common/app_scanner.c
           ../../examples/wireless_examples/bluetooth/common/app_scanner.h
           ../../examples/wireless_examples/bluetooth/common/app_nvm.c
           bluetooth/examples/w_uart/app_config.c
           bluetooth/examples/w_uart/gatt_db.h
           bluetooth/examples/w_uart/gatt_uuid128.h
           bluetooth/examples/w_uart/wireless_uart.c
           bluetooth/examples/w_uart/wireless_uart.h
           bluetooth/examples/w_uart/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/w_uart/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/w_uart/
        ../../examples/wireless_examples/bluetooth/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/w_uart/${multicore_foldername}/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/wireless_uart_${board}.srec
)

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wireless_uart_genfsk_bm LANGUAGES C CXX ASM 
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
           bluetooth/application/common/genfsk_app/genfsk_app.c
           bluetooth/application/common/genfsk_app/genfsk_app.h
           bluetooth/examples/w_uart_genfsk/app_config.c
           bluetooth/examples/w_uart_genfsk/gatt_db.h
           bluetooth/examples/w_uart_genfsk/gatt_uuid128.h
           bluetooth/examples/w_uart_genfsk/wireless_uart.c
           bluetooth/examples/w_uart_genfsk/wireless_uart.h
           bluetooth/examples/w_uart_genfsk/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/w_uart_genfsk/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/w_uart_genfsk/
        bluetooth/application/common/
        bluetooth/application/common/genfsk_app
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/boards/${board}/bluetooth/w_uart_genfsk/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/w_uart_genfsk/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/wireless_uart_genfsk_${board}.srec
)

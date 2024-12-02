cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wireless_uart_genfsk_freertos LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples 
    CUSTOM_PRJ_CONF_PATH examples/_boards/${board}/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/freertos
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    SOURCES
           ../../../../wireless_examples/bluetooth/common/freertos/main.c
           ../../../../wireless_examples/bluetooth/common/freertos/FreeRTOSConfig.h
           ../../../../wireless_examples/bluetooth/common/app_conn.c
           ../../../../wireless_examples/bluetooth/common/app_conn.h
           ../../../../wireless_examples/bluetooth/common/ble_init.c
           ../../../../wireless_examples/bluetooth/common/ble_init.h
           ../../../../wireless_examples/bluetooth/common/app_advertiser.c
           ../../../../wireless_examples/bluetooth/common/app_advertiser.h
           ../../../../wireless_examples/bluetooth/common/app_connection.c
           ../../../../wireless_examples/bluetooth/common/app_scanner.c
           ../../../../wireless_examples/bluetooth/common/app_scanner.h
           ../../../../wireless_examples/bluetooth/common/app_nvm.c
           ../../../../wireless_examples/bluetooth/common/app_low_power.c
           ../../../../wireless_examples/bluetooth/common/genfsk_app/genfsk_app.c
           ../../../../wireless_examples/bluetooth/common/genfsk_app/genfsk_app.h
           ../app_config.c
           ../gatt_db.h
           ../gatt_uuid128.h
           ../wireless_uart.c
           ../wireless_uart.h
)

# workaround
mcux_add_include(
    INCLUDES
            ../../../../wireless_examples/bluetooth/common/freertos/
)
# end workaround

mcux_add_include(
    INCLUDES .
             ../
             ../../common
             ../../../../wireless_examples/bluetooth/common/genfsk_app
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/w_uart_genfsk/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/wireless_uart_genfsk_${board}.srec
)

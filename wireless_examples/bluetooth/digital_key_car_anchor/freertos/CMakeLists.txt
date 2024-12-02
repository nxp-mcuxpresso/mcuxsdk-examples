cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(digital_key_car_anchor_freertos LANGUAGES C CXX ASM 
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
           ../app_config.c
           ../gatt_db.h
           ../gatt_uuid128.h
           ../app_digital_key_car_anchor.c
           ../app_digital_key_car_anchor.h
           ../digital_key_car_anchor.c
           ../digital_key_car_anchor.h
           ../shell_digital_key_car_anchor.c
           ../shell_digital_key_car_anchor.h
           ../../../../_boards/${board}/wireless_examples/bluetooth/digital_key_car_anchor/example_board_readme.md
           ../readme.md
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
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/digital_key_car_anchor/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/digital_key_car_anchor_${board}.srec
)

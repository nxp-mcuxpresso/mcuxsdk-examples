cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(digital_key_device_freertos LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples 
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
           ../../examples/wireless_examples/bluetooth/common/freertos/main.c
           ../../examples/wireless_examples/bluetooth/common/freertos/FreeRTOSConfig.h
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
           ../../examples/wireless_examples/bluetooth/common/app_low_power.c
           bluetooth/examples/digital_key_device/app_config.c
           bluetooth/examples/digital_key_device/gatt_db.h
           bluetooth/examples/digital_key_device/gatt_uuid128.h
           bluetooth/examples/digital_key_device/app_digital_key_device.c
           bluetooth/examples/digital_key_device/app_digital_key_device.h
           bluetooth/examples/digital_key_device/digital_key_device.c
           bluetooth/examples/digital_key_device/digital_key_device.h
           bluetooth/examples/digital_key_device/shell_digital_key_device.c
           bluetooth/examples/digital_key_device/shell_digital_key_device.h
           bluetooth/examples/digital_key_device/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/digital_key_device/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/digital_key_device/
        ../../examples/wireless_examples/bluetooth/common/freertos
        ../../examples/wireless_examples/bluetooth/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/digital_key_device/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/digital_key_device_${board}.srec
)

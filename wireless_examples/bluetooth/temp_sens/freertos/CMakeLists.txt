cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(temperature_sensor_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
        ${board_root}/${board}/wireless_examples/bluetooth/temp_sens/${core_id}/freertos
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/bluetooth/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    SOURCES
           bluetooth/application/common/freertos/main.c
           bluetooth/application/common/freertos/FreeRTOSConfig.h
           bluetooth/application/common/app_conn.c
           bluetooth/application/common/app_conn.h
           bluetooth/application/common/ble_init.c
           bluetooth/application/common/ble_init.h
           bluetooth/application/common/app_connection.c
           bluetooth/application/common/app_advertiser.c
           bluetooth/application/common/app_advertiser.h
           bluetooth/application/common/app_nvm.c
           bluetooth/application/common/app_low_power.c
           bluetooth/examples/temp_sens/app_config.c
           bluetooth/examples/temp_sens/gatt_db.h
           bluetooth/examples/temp_sens/gatt_uuid128.h
           bluetooth/examples/temp_sens/temperature_sensor.c
           bluetooth/examples/temp_sens/temperature_sensor.h
           bluetooth/examples/temp_sens/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/temp_sens/example_board_readme.md
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/examples/temp_sens/
        bluetooth/application/common/freertos
        bluetooth/application/common/
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/temp_sens/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/temperature_sensor_${board}.srec
)

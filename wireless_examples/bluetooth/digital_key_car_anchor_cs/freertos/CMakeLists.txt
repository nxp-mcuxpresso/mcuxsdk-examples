cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(digital_key_car_anchor_cs_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
        ${board_root}/${board}/wireless_examples/bluetooth/digital_key_car_anchor_cs/
        ${board_root}/${board}/wireless_examples/bluetooth/digital_key_car_anchor_cs/${core_id}/freertos
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
           bluetooth/application/common/app_advertiser.c
           bluetooth/application/common/app_advertiser.h
           bluetooth/application/common/app_connection.c
           bluetooth/application/common/app_scanner.c
           bluetooth/application/common/app_scanner.h
           bluetooth/application/common/app_nvm.c
           bluetooth/application/common/app_low_power.c
           bluetooth/examples/digital_key_car_anchor_cs/app_config.c
           bluetooth/examples/digital_key_car_anchor_cs/gatt_db.h
           bluetooth/examples/digital_key_car_anchor_cs/gatt_uuid128.h
           bluetooth/examples/digital_key_car_anchor_cs/app_digital_key_car_anchor_cs.c
           bluetooth/examples/digital_key_car_anchor_cs/app_digital_key_car_anchor_cs.h
           bluetooth/examples/digital_key_car_anchor_cs/digital_key_car_anchor_cs.c
           bluetooth/examples/digital_key_car_anchor_cs/digital_key_car_anchor_cs.h
           bluetooth/examples/digital_key_car_anchor_cs/shell_digital_key_car_anchor_cs.c
           bluetooth/examples/digital_key_car_anchor_cs/shell_digital_key_car_anchor_cs.h
           ../../examples/wireless_examples/bluetooth/digital_key_car_anchor_cs/readme.md
           ../../${board_root}/${board}/wireless_examples/bluetooth/digital_key_car_anchor_cs/example_board_readme.md
)

# workaround
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/application/common/freertos/
)
# end workaround

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/
    INCLUDES
        bluetooth/application/common
        bluetooth/examples/digital_key_car_anchor_cs
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/${bluetooth_root}/boards/${board}/bluetooth/digital_key_car_anchor_cs/app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/digital_key_car_anchor_cs/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/digital_key_car_anchor_cs_${board}.srec
)

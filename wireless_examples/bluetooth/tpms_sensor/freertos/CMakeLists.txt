cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tpms_sensor_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/bluetooth/
        ${board_root}/${board}/wireless_examples/freertos
        examples/_boards/${board}/wireless_examples/bluetooth/tpms_sensor/
        examples/_boards/${board}/wireless_examples/bluetooth/tpms_sensor/${core_id}/freertos
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
           bluetooth/examples/tpms_sensor/app_config.c
           bluetooth/examples/tpms_sensor/gatt_db.h
           bluetooth/examples/tpms_sensor/gatt_uuid128.h
           bluetooth/examples/tpms_sensor/tpms_sensor.c
           bluetooth/examples/tpms_sensor/tpms_sensor.h
           ../../examples/wireless_examples/bluetooth/tpms_sensor/readme.md
           ../../examples/_boards/${board}/wireless_examples/bluetooth/tpms_sensor/example_board_readme.md
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
        bluetooth/examples/tpms_sensor
)

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}
    SOURCES  middleware/wireless/bluetooth/boards/${board}/bluetooth/tpms_sensor/app_preinclude.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/tpms_sensor/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tpms_sensor_${board}.srec
)

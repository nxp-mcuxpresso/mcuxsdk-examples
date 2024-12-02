cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(adv_ext_peripheral_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
    CUSTOM_PRJ_CONF_PATH examples/_boards/${board}/wireless_examples/bluetooth/
        examples/_boards/${board}/wireless_examples/freertos
        examples/_boards/${board}/wireless_examples/bluetooth/adv_ext_peripheral/${core_id}/freertos
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
           ../../../../wireless_examples/bluetooth/common/app_connection.c
           ../../../../wireless_examples/bluetooth/common/app_advertiser.c
           ../../../../wireless_examples/bluetooth/common/app_advertiser.h
           ../../../../wireless_examples/bluetooth/common/app_nvm.c
           ../../../../wireless_examples/bluetooth/common/app_low_power.c
           ../app_config.c
           ../gatt_db.h
           ../gatt_uuid128.h
           ../adv_ext_peripheral.c
           ../adv_ext_peripheral.h
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

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/adv_ext_peripheral/${multicore_foldername}/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(
        TOOLCHAINS armgcc iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/adv_ext_peripheral_${board}.srec
)

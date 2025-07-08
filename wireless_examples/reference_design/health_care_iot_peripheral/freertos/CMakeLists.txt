cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(health_care_iot_peripheral_freertos LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_peripheral
        CUSTOM_PRJ_CONF_PATH examples/wireless_examples/reference_design/health_care_iot_common
                examples/wireless_examples/reference_design/health_care_iot_peripheral/freertos
)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wireless/bluetooth/application/common/freertos/FreeRTOSConfig.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wireless/bluetooth/application/common/freertos
)

include(${SdkRootDirPath}/examples/wireless_examples/reference_design/health_care_iot_peripheral/hciot_peripheral.cmake)

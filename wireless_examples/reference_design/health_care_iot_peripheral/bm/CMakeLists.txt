cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(health_care_iot_peripheral_bm LANGUAGES C CXX ASM
        PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_peripheral
        CUSTOM_PRJ_CONF_PATH examples/wireless_examples/reference_design/health_care_iot_common
                examples/wireless_examples/reference_design/health_care_iot_peripheral/bm
)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples/wireless_examples/reference_design/health_care_iot_peripheral/hciot_peripheral.cmake)
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(health_care_iot_central LANGUAGES C CXX ASM
        PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_central
        CUSTOM_PRJ_CONF_PATH examples/wireless_examples/reference_design/health_care_iot_common
)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples/wireless_examples/reference_design/health_care_iot_central/hciot_central.cmake)

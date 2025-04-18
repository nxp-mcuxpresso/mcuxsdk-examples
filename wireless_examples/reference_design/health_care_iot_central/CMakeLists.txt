cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(health_care_iot_central LANGUAGES C CXX ASM 
        PROJECT_BOARD_PORT_PATH ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_central
        CUSTOM_PRJ_CONF_PATH examples/wireless_examples/reference_design/health_care_iot_common
)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_configuration(
    CC "-DPRINTF_FLOAT_ENABLE"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/wireless_examples/reference_design/health_care_iot_central/main.c
    SOURCES examples/wireless_examples/reference_design/health_care_iot_common/hciot_common.c
    SOURCES examples/wireless_examples/reference_design/health_care_iot_common/hciot_common.h
    SOURCES examples/wireless_examples/reference_design/health_care_iot_central/app_config.c
    SOURCES examples/wireless_examples/reference_design/health_care_iot_central/gatt_db.h
    SOURCES examples/wireless_examples/reference_design/health_care_iot_central/gatt_uuid128.h
    SOURCES examples/wireless_examples/reference_design/health_care_iot_common/msgq.c
    SOURCES examples/wireless_examples/reference_design/health_care_iot_common/msgq.h
    SOURCES middleware/wireless/bluetooth/application/common/app_nvm.c
    SOURCES middleware/wireless/bluetooth/boards/${board}/app_preinclude_common.h           
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/wireless_examples/reference_design/health_care_iot_central
    INCLUDES examples/wireless_examples/reference_design/health_care_iot_common
    INCLUDES ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_central
    INCLUDES middleware/wireless/bluetooth/boards/${board}
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/reference_design/health_care_iot_central/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

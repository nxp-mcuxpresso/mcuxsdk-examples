mcux_add_configuration(
    CC "-DPRINTF_ADVANCED_ENABLE -DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_DEBUG_UART_BAUDRATE=460800"
)

mcux_add_linker_symbol(
    SYMBOLS
        "gUseDatastoreLink_d=1 \
         gUseEventstoreLink_d=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/wireless_examples/reference_design/health_care_iot_peripheral/main.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/app_config.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/gatt_db.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/gatt_uuid128.h
            examples/wireless_examples/reference_design/health_care_iot_common/hciot_common.c
            examples/wireless_examples/reference_design/health_care_iot_common/hciot_common.h
            examples/wireless_examples/reference_design/health_care_iot_common/msgq.c
            examples/wireless_examples/reference_design/health_care_iot_common/msgq.h
            middleware/wireless/bluetooth/application/common/app_nvm.c
            middleware/wireless/bluetooth/boards/${board}/app_preinclude_common.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/blemgr.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/blemgr.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/safety.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/safety.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/connectivity.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/connectivity.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/datastore.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/datastore.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/eventstore.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/eventstore.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/health.c
            examples/wireless_examples/reference_design/health_care_iot_peripheral/health.h
            examples/wireless_examples/reference_design/health_care_iot_peripheral/measurement.h
            ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_peripheral/measurement.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/wireless_examples/reference_design/health_care_iot_peripheral
             examples/wireless_examples/reference_design/health_care_iot_common
             ${board_root}/${board}/wireless_examples/reference_design/health_care_iot_peripheral
             middleware/wireless/bluetooth/boards/${board}
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/reference_design/health_care_iot_peripheral/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

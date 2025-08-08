mcux_add_configuration(
    CC "-DSERIAL_USE_CONFIGURE_STRUCTURE=1\
       -DgButtonSupported_d=1\
       -DOSA_USED\
       -DSDK_DEBUGCONSOLE_UART\
       -DSDK_COMPONENT_INTEGRATION=1\
       -DFSL_OSA_TASK_ENABLE=1\
       -DCR_INTEGER_PRINTF\
       -DCFG_BLE_PRJ=1\
       -DgUseHciTransportDownward_d=1\
       -DHAL_FLASH_ROMAPI_DRIVER=1"
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

if(CONFIG_MCUX_HW_DEVICE_ID_MCXW235)
    message(FATAL_ERROR "MCXW235 SoC variant is not supported for OTA examples.")
endif()

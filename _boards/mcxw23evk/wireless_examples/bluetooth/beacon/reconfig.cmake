mcux_add_configuration(
    CC "-DgButtonSupported_d=1\
       -DOSA_USED\
       -DFSL_OSA_TASK_ENABLE=1\
       -DCR_INTEGER_PRINTF\
       -DCFG_BLE_PRJ=1\
       -DHAL_FLASH_ROMAPI_DRIVER=1"
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
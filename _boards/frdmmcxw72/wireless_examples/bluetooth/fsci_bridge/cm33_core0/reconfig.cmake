mcux_add_configuration(
    CC "-DSERIAL_USE_CONFIGURE_STRUCTURE=1\
       -DgButtonSupported_d=1\
       -DSDK_DEBUGCONSOLE=2\
       -DOSA_USED\
       -DSDK_DEBUGCONSOLE_UART\
       -DSDK_COMPONENT_INTEGRATION=1\
       -DFSL_OSA_TASK_ENABLE=1\
       -DCR_INTEGER_PRINTF\
       -DMCXW727C\
       -DCPU_MCXW727CMFTA_cm33_core0\
       -DCFG_BLE_PRJ=1\
       -DENABLE_RAM_VECTOR_TABLE=1\
       -DNXP_SSSAPI\
       -DNXP_ELE200\
       -DHAL_FLASH_ROMAPI_DRIVER=1\
       -DTM_ENABLE_TIME_STAMP=1"
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pa082,Pa050"
    LD "--config_def=gUseNVMLink_d=1\
        --config_def=gEraseNVMLink_d=1\
        --config_def=__ram_vector_table__=1\
        --config_def=gUseProdInfoLegacyMode_d=1\
        --config_def=__use_shmem__=1\
        --config_def=gLowpowerPowerDownEnable_d=0"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=gUseNVMLink_d=1\
        -Xlinker --defsym=gEraseNVMLink_d=1\
        -Xlinker --defsym=__ram_vector_table__=1\
        -Xlinker --defsym=gUseProdInfoLegacyMode_d=1\
        -Xlinker --defsym=__use_shmem__=1\
	-Xlinker --defsym=gLowpowerPowerDownEnable_d=0\
        -Xlinker --defsym=PROD_DATA_BASE_ADDR=0xfe000\
        -Xlinker --defsym=__base_PROD_DATA=0xfe000"
)



mcux_add_macro(ECC_RAM_STCM8_INIT=1)
mcux_add_configuration(
    CC "-DSERIAL_USE_CONFIGURE_STRUCTURE=1\
       -DgButtonSupported_d=1\
       -DSDK_DEBUGCONSOLE=2\
       -DOSA_USED\
       -DSDK_DEBUGCONSOLE_UART\
       -DSDK_COMPONENT_INTEGRATION=1\
       -DFSL_OSA_TASK_ENABLE=1\
       -DCR_INTEGER_PRINTF\
       -DMCXW716C\
       -DCPU_MCXW716CMFTA\
       -DCFG_BLE_PRJ=1\
       -DENABLE_RAM_VECTOR_TABLE=1\
       -DNXP_SSSAPI\
       -DNXP_ELE200\
       -DHAL_FLASH_ROMAPI_DRIVER=1\
       -DgUseHciTransportDownward_d=1\
       -DTM_ENABLE_TIME_STAMP=1\
       -DBOARD_FRDMCXW71_SUPPORT=1\
       -DHAL_UART_ADAPTER_LOWPOWER=1"
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pa082,Pa050"
    LD "--config_def=gUseNVMLink_d=1\
       --config_def=gEraseNVMLink_d=1\
       --config_def=__ram_vector_table__=1\
       --config_def=gFlashNbuImage_d=1\
       --config_def=gUseProdInfoLegacyMode_d=1\
       --config_def=__use_shmem__=1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=gUseNVMLink_d=1\
        -Xlinker --defsym=gEraseNVMLink_d=1\
        -Xlinker --defsym=__ram_vector_table__=1\
        -Xlinker --defsym=gFlashNbuImage_d=1\
        -Xlinker --defsym=gUseProdInfoLegacyMode_d=1\
        -Xlinker --defsym=__use_shmem__=1"
)

mcux_add_source(
    SOURCES ../../app_preinclude_common.h
        app_preinclude.h
        readme.md
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES app_preinclude.h
)

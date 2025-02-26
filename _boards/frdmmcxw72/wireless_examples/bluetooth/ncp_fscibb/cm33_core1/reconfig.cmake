mcux_add_configuration(
    CC "-DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=0\
       -DSCANF_ADVANCED_ENABLE=0\
       -DFSL_OSA_TASK_ENABLE=1\
       -DSDK_COMPONENT_INTEGRATION=1\
       -DgUseHciTransportUpward_d=0\
       -DNBU_PROJECT_ENABLE=1\
       -DUSE_RAW_IMU_ADAPTER=0\
       -DCPU_MCXW727CMFTA_cm33_core1\
       -DHAL_RPMSG_SELECT_ROLE=1\
       -DSERIAL_PORT_TYPE_RPMSG=1\
       -DRPMSG_ADAPTER_NON_BLOCKING_MODE=1\
       -DSERIAL_MANAGER_NON_BLOCKING_MODE=1\
       -DHCI_FREE_RxBuffer=0\
       -DOSA_USED\
       -DTIMER_PORT_TYPE_LPTMR=1\
       -DFSL_RTOS_THREADX=1\
       -DRF_OSC_26MHZ=0\
       -DTEST_BUILD_COEX=0\
       -DARM_MATH_CM3=1\
       -DSSS_CONFIG_FILE=\"fsl_sss_config_elemu.h\"\
       -DSSCP_CONFIG_FILE=\"fsl_sscp_config_elemu.h\"\
       -DMULTICORE_APP=1\
       -DTX_INCLUDE_USER_DEFINE_FILE"
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pa082,Pa050"
    LD "--config_def=__use_shmem__=1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__use_shmem__=1"
)


mcux_project_remove_include(
        BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board}
    )

mcux_add_macro(ECC_RAM_STCM8_INIT=1)
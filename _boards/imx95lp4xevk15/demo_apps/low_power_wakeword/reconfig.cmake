
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/cm7/app_srtm.c
            examples/_boards/${board}/cm7/app_srtm.h
            examples/_boards/${board}/demo_apps/low_power_wakeword/cm7/lpm.c
            examples/_boards/${board}/demo_apps/low_power_wakeword/cm7/lpm.h
            examples/_boards/${board}/demo_apps/low_power_wakeword/cm7/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/demo_apps/low_power_wakeword/cm7/rpmsg_config.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/_boards/${board}/cm7/
)

mcux_add_macro(
    CC "-DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf\
       -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN\
       -DNOT_CONFIG_CLK_ROOT=1\
       -DVOICE_ENGINE_CONFIG_VOICESEEKER_2MIC=1\
       -DSRTM_PDM_EDMA_DATA_INJECTION=1\
       -DSRTM_SAI_EDMA_CONFIG_RX_ENABLE=0\
       -DSRTM_PDM_EDMA_ADAPTER_USE_HWVAD=0\
       -DBOARD_USE_PCAL6408=1\
       -DBOARD_USE_PCA6416A\
       -DUSE_SRTM_PDM_EDMA\
       -DFSL_EDMA_DRIVER_EDMA4\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DSRTM_AUDIO_SERVICE_USED=1\
       -DSRTM_SAI_EDMA_PLAY_HALT_WORKAROUND=1\
       -DSRTM_SAI_EDMA_CLOCK_SWITCH_ENABLE=1\
       -DDEMO_SAI_TX_CONFIG_StopOnSuspend=1\
       -DDEMO_SAI_TX_CONFIG_UseLocalBuf=0\
       -DSRTM_SAI_CONFIG_Rx_Enabled=0\
       -DSRTM_SAI_CONFIG_SupportLocalBuf=0"
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS release
    LINKER examples/_boards/${board}/demo_apps/low_power_wakeword/cm7/linker/MIMX9596_cm7_lpv_ram.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x0080\
        -Xlinker --defsym=__heap_size__=0x12C00"
)

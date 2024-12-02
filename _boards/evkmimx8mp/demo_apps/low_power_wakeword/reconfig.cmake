
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
            examples/_boards/${board}/demo_apps/low_power_wakeword/lpm.c
            examples/_boards/${board}/demo_apps/low_power_wakeword/lpm.h
            examples/_boards/${board}/demo_apps/low_power_wakeword/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/demo_apps/low_power_wakeword/rpmsg_config.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
)

mcux_add_macro(
    CC "-DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf\
       -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN\
       -DNOT_CONFIG_CLK_ROOT=1\
       -DSRTM_PDM_SDMA_ADAPTER_USE_HWVAD=0\
       -DSRTM_PDM_SDMA_ADAPTER_FORCE_LOCAL_AND_EXTRA_BUFFERS=1\
       -DDEMO_SAI_TX_CONFIG_StopOnSuspend=1\
       -DDEMO_SAI_TX_CONFIG_UseLocalBuf=0\
       -DSRTM_SAI_CONFIG_Rx_Enabled=0\
       -DSRTM_SAI_CONFIG_SupportLocalBuf=0"
    AS "-DNO_CRP\
      -D__PERFORMANCE_IMPLEMENTATION"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8MP/MIMX8ML8/gcc/MIMX8ML8xxxLZ_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS release
    LINKER examples/_boards/${board}/demo_apps/low_power_wakeword/linker/MIMX8ML8xxxxx_cm7_lpv_ram.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x0080\
        -Xlinker --defsym=__heap_size__=0x12C00"
)

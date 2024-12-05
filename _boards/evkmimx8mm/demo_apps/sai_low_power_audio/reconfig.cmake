
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/rpmsg_config.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/lpm.c
            ${board_root}/${board}/demo_apps/sai_low_power_audio/lpm.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.c
            ${board_root}/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
            ${board_root}/${board}/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
)

mcux_add_macro(
    CC "-DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf\
       -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN\
       -DSRTM_PDM_SDMA_ADAPTER_USE_HWVAD=0\
       -DSRTM_SINGLE_SDMA_MULTI_FIFO_SCRIPT=1\
       -DSRTM_PDM_SDMA_ADAPTER_USE_EXTRA_BUFFER=0\
       -DNOT_CONFIG_CLK_ROOT=1"
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX8MM/MIMX8MM6/gcc/MIMX8MM6xxxLZ_ram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX8MM/MIMX8MM6/iar/MIMX8MM6xxxLZ_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/gcc/MIMX8MM6xxxxx_cm4_lpa_ram.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/iar/MIMX8MM6xxxxx_cm4_lpa_ram.icf
)

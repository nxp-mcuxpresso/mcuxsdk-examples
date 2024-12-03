
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/rpmsg_config.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/lpm.c
            examples/_boards/${board}/demo_apps/sai_low_power_audio/lpm.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.c
            examples/_boards/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
)

mcux_add_macro(
    CC "-DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf\
       -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN"
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8MP/MIMX8ML8/gcc/MIMX8ML8xxxLZ_ram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8MP/MIMX8ML8/iar/MIMX8ML8xxxLZ_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/gcc/MIMX8ML8xxxxx_cm7_lpa_ram.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/iar/MIMX8ML8xxxxx_cm7_lpa_ram.icf
)

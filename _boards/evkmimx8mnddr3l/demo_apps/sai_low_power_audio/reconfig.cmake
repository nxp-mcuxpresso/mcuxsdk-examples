
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            ${board_root}/evkmimx8mn/FreeRTOSConfigBoard.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.c
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/sai_low_power_audio.h
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/pin_mux.c
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/pin_mux.h
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/rpmsg_config.h
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/lpm.c
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/lpm.h
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/hardware_init.c
            ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio/app.h
            ${board_root}/evkmimx8mn/rsc_table.c
            ${board_root}/evkmimx8mn/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/evkmimx8mn/app_srtm.c
            ${board_root}/evkmimx8mn/app_srtm.h
            ${board_root}/evkmimx8mn/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn
             ${board_root}/evkmimx8mn/demo_apps/sai_low_power_audio
             examples/demo_apps/sai_low_power_audio
             middleware/multicore/remoteproc
)

mcux_add_macro(
    CC "-DSRTM_DEBUG_MESSAGE_FUNC=DbgConsole_Printf\
       -DSRTM_DEBUG_VERBOSE_LEVEL=SRTM_DEBUG_VERBOSE_WARN\
       -DNOT_CONFIG_CLK_ROOT=1"
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX8MN/MIMX8MN6/gcc/MIMX8MN6xxxJZ_ddr3l_ram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX8MN/MIMX8MN6/iar/MIMX8MN6xxxJZ_ddr3l_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/gcc/MIMX8MN6xxxxx_cm7_lpa_ram_ddr3l.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/iar/MIMX8MN6xxxxx_cm7_lpa_ram_ddr3l.icf
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

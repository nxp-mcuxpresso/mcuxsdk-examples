
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/rpmsg_config.h
            examples/demo_apps/sai_low_power_audio_imx93/sai_low_power_audio.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/sai_low_power_audio_imx93
             middleware/multicore/remoteproc
)

mcux_add_configuration(
           CC "-DSRTM_SAI_EDMA_LOCAL_BUF_ENABLE=1"
)
mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_DDR_RETENTION=1"
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX93/MIMX9322/iar/MIMX9322xxxxM_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX93/MIMX9322/gcc/MIMX9322xxxxM_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/iar/MIMX9322_cm33_lpa_ram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/gcc/MIMX9322_cm33_lpa_ram.ld
)


mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
            -DMCUX_SDK_EDMA_USE_DMA5=1\
            -DSRTM_SAI_EDMA_CLOCK_SWITCH_ENABLE=1\
            -DSRTM_SAI_EDMA_LOCAL_BUF_ENABLE=1\
            -DSRTM_AUDIO_SERVICE_USED=1\
            -DBOARD_USE_PCAL6408=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cm7_core0/app_srtm.c
            ${board_root}/${board}/cm7_core0/app_srtm.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/cm7_core0/rpmsg_config.h
            examples/demo_apps/sai_low_power_audio_imx943/sai_low_power_audio.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/cm7_core0/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/sai_low_power_audio_imx943
             middleware/multicore/remoteproc
             ${board_root}/${board}/cm7_core0
             ${board_root}/${board}/demo_apps/sai_low_power_audio
             ${board_root}/${board}/demo_apps/sai_low_power_audio/linker
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX943/MIMX94398/iar/MIMX94398_cm7_core0_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX943/MIMX94398/gcc/MIMX94398_cm7_core0_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx943evk/demo_apps/sai_low_power_audio/cm7_core0/linker/MIMX94398_cm7_core0_lpa_ram.ld
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx943evk/demo_apps/sai_low_power_audio/cm7_core0/linker/MIMX94398_cm7_core0_lpa_ram.icf

)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
    )


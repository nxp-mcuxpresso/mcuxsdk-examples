
mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
            -DMCUX_SDK_EDMA_USE_DMA5=1\
            -DSRTM_SAI_EDMA_LOCAL_BUF_ENABLE=1\
            -DSRTM_SAI_EDMA_CLOCK_SWITCH_ENABLE=1\
            -DSRTM_AUDIO_SERVICE_USED=1\
            -DSRTM_SAI_EDMA_PLAY_HALT_WORKAROUND=1\
            -DBOARD_USE_PCAL6408=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cm7/app_srtm.c
            examples/_boards/${board}/cm7/app_srtm.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/cm7/rpmsg_config.h
            examples/demo_apps/sai_low_power_audio_imx95/sai_low_power_audio.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/cm7/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/sai_low_power_audio_imx95
             middleware/multicore/remoteproc
             examples/_boards/${board}/cm7
             examples/_boards/${board}/demo_apps/sai_low_power_audio
             examples/_boards/${board}/demo_apps/sai_low_power_audio/linker
)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)

include(${SdkRootDirPath}/examples/_boards/imx95lpd5evk19/demo_apps/sai_low_power_audio/reconfig.cmake)

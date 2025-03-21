# board+application specific cmake

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/drivers 
    components/video/camera
    components/video/camera/device
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_ov7670.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_sccb.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_ov7670.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_sccb.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/camera_config.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv_para.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv.bin
            ${board_root}/${board}/eiq_examples/mpp/drivers/incbin_cm33_core0.S
            components/video/camera/fsl_camera.h
            components/video/camera/device/fsl_camera_device.h
)


mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=2000 \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DCONFIG_OV7670_FREERUNNING_PCLK=1 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 "
    CX "DconfigGENERATE_RUN_TIME_STATS=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DCONFIG_OV7670_FREERUNNING_PCLK=1 "
)

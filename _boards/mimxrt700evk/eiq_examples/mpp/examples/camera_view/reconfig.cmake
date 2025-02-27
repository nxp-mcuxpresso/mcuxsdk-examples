# board+application specific cmake

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES drivers        
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/src/pin_mux.c
            ${board_root}/${board}/eiq_examples/mpp/src/pmic_support.c
            ${board_root}/${board}/eiq_examples/mpp/src/board.c
            ${board_root}/${board}/eiq_examples/mpp/inc/pin_mux.h
            ${board_root}/${board}/eiq_examples/mpp/inc/pmic_support.h
            ${board_root}/${board}/eiq_examples/mpp/inc/board.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv_support.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_camera_i2c.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_ezhv.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_ov7670.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_sccb.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv_support.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_camera_i2c.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_ezhv.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_ov7670.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/fsl_sccb.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/camera_config.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv_config.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv.bin
            ${board_root}/${board}/eiq_examples/mpp/drivers/incbin_cm33_core0.S
)

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=2000 \
    -DconfigGENERATE_RUN_TIME_STATS=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DSDK_I2C_BASED_COMPONENT_USED=1"
    CX "DconfigGENERATE_RUN_TIME_STATS=1 \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DSDK_I2C_BASED_COMPONENT_USED=1"
)

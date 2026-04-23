# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES drivers
             inc
             inc/${core_id}
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/inc/FreeRTOSConfig.h
            ${board_root}/${board}/eiq_examples/mpp/inc/fsl_debug_console_conf.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/camera_config.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.h
            middleware/eiq/mpp/hal/hal_${board}.c
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc555/0x423_ECO \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
        -DVG_TARGET_FAST_CLEAR=0 \
        -DSSD1963_DATA_WITDH=8 \
        -DSDK_OS_FREE_RTOS \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -D__FPU_PRESENT=1 \
        -DUSB_STACK_FREERTOS \
        -DUSE_RTOS=1 \
        -DPROCESS_IMAGE=1 \
        -DUSE_PSRAM_JPG_BUFFERS=1 \
        -DARM_MATH_CM33"
    CX "-DARM_MATH_CM33\
        -DUSB_STACK_FREERTOS \
        -DUSE_RTOS=1 \
        -DPROCESS_IMAGE=1 \
        -DUSE_PSRAM_JPG_BUFFERS=1 \
        -D__FPU_PRESENT=1"
)

# core specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/${core_id}/reconfig.cmake)

# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES inc
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/src/pin_mux.c
            ${board_root}/${board}/eiq_examples/mpp/src/hardware_init.c
            ${board_root}/${board}/eiq_examples/mpp/src/utick_config.c
            ${board_root}/${board}/eiq_examples/mpp/src/ostimer_config.c
            ${board_root}/${board}/eiq_examples/mpp/inc/ostimer_config.h
            ${board_root}/${board}/eiq_examples/mpp/inc/pin_mux.h
            ${board_root}/${board}/eiq_examples/mpp/inc/app.h
            ${board_root}/${board}/eiq_examples/mpp/inc/utick_config.h
            ${board_root}/${board}/eiq_examples/mpp/inc/FreeRTOSConfig.h
            ${board_root}/${board}/eiq_examples/mpp/inc/fsl_debug_console_conf.h
            middleware/eiq/mpp/hal/hal_${board}.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.c
            ${board_root}/${board}/eiq_examples/mpp/drivers/camera_config.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/flexio_camera.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv_para.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv.bin
            ${board_root}/${board}/eiq_examples/mpp/drivers/incbin_cm33_core0.S
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/drivers
             ${board_root}/${board}/eiq_examples/mpp/inc
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc555/0x423_ECO \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1 \
        -DVG_TARGET_FAST_CLEAR=0 \
        -DSSD1963_DATA_WITDH=8 \
        -DSDK_OS_FREE_RTOS \
        -DBOARD_ENABLE_PSRAM_CACHE=0 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -D__FPU_PRESENT=1 \
        -DUSB_STACK_FREERTOS \
        -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
        -DUSE_RTOS=1 \
        -DPROCESS_IMAGE=1 \
        -DUSE_PSRAM_JPG_BUFFERS=1 \
        -DARM_MATH_CM33"
    CX "-DARM_MATH_CM33\
        -DUSB_STACK_FREERTOS \
        -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 \
        -DUSE_RTOS=1 \
        -DPROCESS_IMAGE=1 \
        -DUSE_PSRAM_JPG_BUFFERS=1 \
        -D__FPU_PRESENT=1"
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    LINKER MIMXRT798Sxxxx_cm33_core0_flash.ld
)


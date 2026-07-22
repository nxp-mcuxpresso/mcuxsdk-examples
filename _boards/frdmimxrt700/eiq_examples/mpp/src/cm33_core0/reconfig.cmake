# list core specific files for SDK

# Exclude SDK's display_support.c — MPP provides its own for the Waveshare 5inch DSI panel
set_source_files_properties(
    ${SdkRootDirPath}/${board_root}/${board}/display_support.c
    PROPERTIES HEADER_FILE_ONLY TRUE
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv_para.h
            ${board_root}/${board}/eiq_examples/mpp/drivers/ezhv.bin
            ${board_root}/${board}/eiq_examples/mpp/drivers/incbin_cm33_core0.S
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/hardware_init.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/pin_mux.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/utick_config.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/ostimer_config.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/display_support.c
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/app.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/pin_mux.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/utick_config.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/ostimer_config.h
            ${board_root}/${board}/eiq_examples/mpp/inc/display_support.h
            middleware/eiq/mpp/hal/hal_display_lcdif_waveshare5inch.c
)

mcux_add_macro(
    CC "-DBOARD_ENABLE_PSRAM_CACHE=0 \
        -DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
    CX "-DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
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
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/${core_id}
    LINKER MIMXRT798Sxxxx_cm33_core0_flash.ld
)

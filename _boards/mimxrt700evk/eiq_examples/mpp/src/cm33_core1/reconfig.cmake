# core specific cmake

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/hardware_init.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/pin_mux.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/ostimer_config.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/utick_config.c
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/app.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/pin_mux.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/ostimer_config.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/utick_config.h
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/${core_id}
    LINKER MIMXRT798Sxxxx_cm33_core1_ram.ld
)

# list core specific files for SDK

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/hardware_init.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/pin_mux.c
            ${board_root}/${board}/eiq_examples/mpp/src/${core_id}/gpt_config.c
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/app.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/pin_mux.h
            ${board_root}/${board}/eiq_examples/mpp/inc/${core_id}/dcd.h
)

mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_sdram.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/src/${core_id}
    LINKER MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.ld
)

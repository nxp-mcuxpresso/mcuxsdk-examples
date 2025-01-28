# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    INCLUDES inc
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    SOURCES src/pin_mux.c
            src/hardware_init.c
            src/gpt_config.c
            inc/pin_mux.h
            inc/dcd.h
            inc/app.h
            inc/FreeRTOSConfig.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/display_support.h
            ${board_root}/${board}/display_support.c
            ${board_root}/${board}/camera_support.h
            ${board_root}/${board}/camera_support.c
            middleware/eiq/mpp/hal/hal_${board}.c
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc355/0x0_1216 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1"
)

mcux_remove_armgcc_linker_script(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_sdram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp
    LINKER MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.ld
)

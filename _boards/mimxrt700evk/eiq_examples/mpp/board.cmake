# lists board specific files for SDK

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp
    INCLUDES inc
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp
    SOURCES src/pin_mux.c
            src/hardware_init.c
            src/clock_config.c
            src/utick_config.c
            inc/pin_mux.h
            inc/app.h
            inc/utick_config.h
            inc/FreeRTOSConfig.h
            inc/clock_config.h
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc555/0x423_ECO \
        -DMCUX_DBI_LEGACY=0 \
        -DDBI_USE_MIPI_PANEL=1 \
        -DSDK_I2C_BASED_COMPONENT_USED=1 \
        -DSSD1963_DATA_WITDH=8 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=1"
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/mpp
    LINKER MIMXRT798Sxxxx_cm33_core0_flash.ld
)

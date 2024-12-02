mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
            -DBOARD_ENABLE_PSRAM_CACHE=0\
            -DSSD1963_DATA_WITDH=8\
            -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8\
            -DMCUX_DBI_LEGACY=0\
            -DDBI_USE_MIPI_PANEL=1\
            -DSDK_DEBUGCONSOLE_UART=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.ld
)


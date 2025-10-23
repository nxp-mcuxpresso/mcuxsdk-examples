mcux_add_iar_configuration(
    CC "--diag_suppress Pa082,Pa050,Pa092"
    CX "--diag_suppress Pa082,Pa050,Pa092"
)

mcux_add_iar_configuration(
    TARGETS debug release flash_debug flash_release
    LD "--semihosting\
       --redirect __iar_sh_stdout=__iar_sh_stdout_swo"
)

mcux_add_macro(
    CC "-DGCID_REV_CID=gc255/0x40A \
        -DBOARD_ENABLE_PSRAM_CACHE=0 \
        -DSSD1963_DATA_WITDH=8 \
        -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
        -DSDK_DEBUGCONSOLE_UART \
        -DDBI_FLEXIO_USE_SMARTDMA=1 \
        -DCUSTOM_VGLITE_MEMORY_CONFIG=0"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_ram.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_ram.scf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_ram.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT595Sxxxx_cm33_ram.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT595Sxxxx_cm33_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/lvgl_examples/linker/MIMXRT595Sxxxx_cm33_ram.ld
)

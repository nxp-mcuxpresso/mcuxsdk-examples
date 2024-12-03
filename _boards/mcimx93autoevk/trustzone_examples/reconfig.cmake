# Remove no_se from IAR FLags
mcux_remove_iar_configuration(
    AS "--cpu=cortex-m33.no_se"
    CC "--cpu=cortex-m33.no_se"
    CX "--cpu=cortex-m33.no_se"
    LD "--cpu=cortex-m33.no_se"
)

# And then this will add Trust Zone Enable clicky button checked in IAR GUI Project
mcux_add_iar_configuration(
    AS "--cpu=cortex-m33"
    CC "--cpu=cortex-m33"
    CX "--cpu=cortex-m33"
    LD "--cpu=cortex-m33"
)

# Remove the default linkers; replaced by the TZM ones in nested cmakes
mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

mcux_remove_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)

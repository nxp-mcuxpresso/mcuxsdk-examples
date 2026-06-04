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

mcux_add_armgcc_configuration(
    CX "-Wno-sign-compare\
        -Wno-format\
        -Wno-shift-count-overflow\
        -Wno-deprecated-declarations\
        -Wno-int-in-bool-context"
)

mcux_add_mcux_configuration(
    CX "-Wno-sign-compare\
        -Wno-format\
        -Wno-shift-count-overflow\
        -Wno-deprecated-declarations\
        -Wno-int-in-bool-context"
)

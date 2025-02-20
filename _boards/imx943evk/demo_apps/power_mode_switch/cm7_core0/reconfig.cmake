
# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX943/MIMX94398/iar/MIMX94398_cm7_core0_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX943/MIMX94398/gcc/MIMX94398_cm7_core0_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx943evk/demo_apps/power_mode_switch/cm7_core0/linker/MIMX94398_cm7_core0_ram_pms.ld
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx943evk/demo_apps/power_mode_switch/cm7_core0/linker/MIMX94398_cm7_core0_ram_pms.icf

)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
    )


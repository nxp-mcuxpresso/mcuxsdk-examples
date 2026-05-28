
# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX952/MIMX9529/iar/MIMX9529_cm7_ram.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX952/MIMX9529/gcc/MIMX9529_cm7_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx952/demo_apps/sai_low_power_audio/linker/MIMX95294_cm7_lpa_ram.ld
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
	LINKER ${board_root}/frdmimx952/demo_apps/sai_low_power_audio/linker/MIMX95294_cm7_lpa_ram.icf

)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)



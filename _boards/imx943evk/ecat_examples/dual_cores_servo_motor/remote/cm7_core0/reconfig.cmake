# Add additional configuration
mcux_project_remove_source(
  SOURCES
	clock_config.h
	clock_config.c
)

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

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/MIMX94398_cm7_core0_ram.icf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/remote/${core_id}/MIMX94398_cm7_core0_ram.ld
)

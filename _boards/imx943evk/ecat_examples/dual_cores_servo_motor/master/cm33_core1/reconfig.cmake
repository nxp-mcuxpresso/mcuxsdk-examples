# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX943/MIMX94398/iar/MIMX94398_cm33_core1_ram.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX943/MIMX94398/gcc/MIMX94398_cm33_core1_ram.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/${core_id}/MIMX94398_cm33_core1_ram.icf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/${core_id}/MIMX94398_cm33_core1_ram.ld
)

mcux_add_armgcc_configuration(
    TARGETS debug release
    CC "-O0"
)

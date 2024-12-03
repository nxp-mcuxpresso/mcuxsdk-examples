mcux_set_variable(CONFIG_ZB_PLATFORM "RW612")

#zigbee applications defines
mcux_add_macro(
    gAppLedCnt_c=2
    SERIAL_MANAGER_TASK_PRIORITY=8
    TM_TASK_PRIORITY=6
    PRINTF_ADVANCED_ENABLE=1
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --sort-section=alignment"
)

mcux_add_linker_symbol(
    SYMBOLS
        "gUseNVMLink_d=1 \
        gNVMSectorCountLink_d=8"
)

mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/wireless_examples
    LINKER linker/gcc/RW61x_flash.ld
)

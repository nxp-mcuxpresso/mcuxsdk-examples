mcux_add_source(
    PREINCLUDE TRUE
    SOURCES app_preinclude.h
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/MCXW236_flash.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/wireless_examples/linker/gcc/connectivity_ble.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/wireless_examples/linker/iar/${iar_wireless_linker_file}
)

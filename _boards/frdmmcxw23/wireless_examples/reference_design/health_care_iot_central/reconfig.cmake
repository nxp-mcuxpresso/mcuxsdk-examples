mcux_add_source(
    PREINCLUDE TRUE
    SOURCES app_preinclude.h
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/mcxw23
    TARGETS debug release
    LINKER linker/gcc/${gcc_wireless_linker_file_ble}
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/mcxw23
    TARGETS debug release
    LINKER linker/iar/${iar_wireless_linker_file}
)

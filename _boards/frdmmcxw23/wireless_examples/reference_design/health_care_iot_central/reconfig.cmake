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

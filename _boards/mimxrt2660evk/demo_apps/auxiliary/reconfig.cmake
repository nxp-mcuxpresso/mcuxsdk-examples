
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_only_debug psram_only_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram_only.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_only_debug psram_only_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_ram_only.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_only_debug psram_only_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram_only.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_only_debug psram_only_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_ram_only.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_only_debug psram_only_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/arm/MIMXRT2663xxxxx_psram_only.scf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_only_debug psram_only_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/arm/MIMXRT2663xxxxx_ram_only.scf
)

mcux_remove_configuration(
    TARGETS psram_only_debug psram_only_release
    CC "-DENABLE_RAM_VECTOR_TABLE"
    CX "-DENABLE_RAM_VECTOR_TABLE"
)
mcux_remove_iar_configuration(
    TARGETS psram_only_debug psram_only_release
    LD "--config_def=__ram_vector_table__=1"
)
mcux_remove_mdk_configuration(
    TARGETS psram_only_debug psram_only_release
    LD "--predefine=\"-D__ram_vector_table__=1\""
)
mcux_remove_armgcc_configuration(
    TARGETS psram_only_debug psram_only_release
    LD "-Xlinker --defsym=__ram_vector_table__=1"
)

mcux_remove_configuration(
    TARGETS psram_only_debug             psram_only_release
    CC "-DUSDHC_BOOT_HEADER_ENABLE=0"
)
mcux_add_configuration(
    TARGETS psram_only_debug             psram_only_release
    CC "-DUSDHC_BOOT_HEADER_ENABLE=1"
)
mcux_add_mdk_configuration(
    TARGETS psram_only_debug             psram_only_release
    LD "--predefine=\"-D__usdhc_boot_header_enable__=1\""
)
mcux_add_iar_configuration(
    TARGETS psram_only_debug             psram_only_release
    LD "--config_def=__usdhc_boot_header_enable__=1"
)
mcux_add_armgcc_configuration(
    TARGETS psram_only_debug             psram_only_release
    LD "-Xlinker --defsym=__usdhc_boot_header_enable__=1"
)
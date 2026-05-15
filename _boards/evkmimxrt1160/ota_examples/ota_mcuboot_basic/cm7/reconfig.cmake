# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DUSE_SDRAM\
        -DDATA_SECTION_IS_CACHEABLE=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1166xxxxx_cm7_flexspi_nor_mcuboot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1166xxxxx_cm7_flexspi_nor_mcuboot.ld
)

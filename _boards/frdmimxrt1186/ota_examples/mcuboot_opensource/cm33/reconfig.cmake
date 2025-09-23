
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-Og"
)



# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O0"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1186/iar/MIMXRT1186xxxxx_cm33_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1186/gcc/MIMXRT1186xxxxx_cm33_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1186xxxxx_cm33_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1186xxxxx_cm33_flexspi_nor.ld
)

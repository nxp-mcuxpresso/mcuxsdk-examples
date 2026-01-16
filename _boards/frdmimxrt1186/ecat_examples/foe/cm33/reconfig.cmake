
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
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1186xxxxx_cm33_flexspi_nor_mcuboot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1186xxxxx_cm33_flexspi_nor_mcuboot.ld
)

mcux_remove_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    CC "-DXIP_BOOT_HEADER_ENABLE=1"
)

mcux_add_configuration(
    TARGETS flexspi_nor_debug           flexspi_nor_release
    CC "-DXIP_BOOT_HEADER_ENABLE=0"
)

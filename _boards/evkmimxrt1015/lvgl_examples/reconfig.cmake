
mcux_add_macro(
  CC "-DXIP_BOOT_HEADER_ENABLE=1"
  TARGETS flexspi_nor_debug flexspi_nor_release
  TOOLCHAINS mdk
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1015/MIMXRT1015/iar/MIMXRT1015xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1015/MIMXRT1015/arm/MIMXRT1015xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1015/MIMXRT1015/gcc/MIMXRT1015xxxxx_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1015/MIMXRT1015/iar/MIMXRT1015xxxxx_flexspi_nor_ocram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1015/MIMXRT1015/arm/MIMXRT1015xxxxx_flexspi_nor_ocram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1015/MIMXRT1015/gcc/MIMXRT1015xxxxx_flexspi_nor_ocram.ld
)

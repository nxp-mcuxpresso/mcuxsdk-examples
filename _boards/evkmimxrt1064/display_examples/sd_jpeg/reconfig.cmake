
# Add additional configuration
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "--keep=*(.boot_hdr.dcd_data)"
)
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DSKIP_SYSCLK_INIT\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1064/MIMXRT1064/iar/MIMXRT1064xxxxx_flexspi_nor.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1064/MIMXRT1064/arm/MIMXRT1064xxxxx_flexspi_nor.scf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1064/MIMXRT1064/gcc/MIMXRT1064xxxxx_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1064/MIMXRT1064/iar/MIMXRT1064xxxxx_flexspi_nor_sdram.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1064/MIMXRT1064/arm/MIMXRT1064xxxxx_flexspi_nor_sdram.scf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1064/MIMXRT1064/gcc/MIMXRT1064xxxxx_flexspi_nor_sdram.ld
)

mcux_add_iar_configuration(
    TARGETS sdram_debug
    CC "-Om"
)

mcux_remove_iar_configuration(
    TARGETS sdram_debug
    CC "-On"
)

mcux_add_iar_configuration(
    TARGETS flexspi_nor_debug
    CC "-Om"
)

mcux_remove_iar_configuration(
    TARGETS flexspi_nor_debug
    CC "-On"
)

mcux_add_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O1"
    CX "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS sdram_debug
    CC "-O0"
    CX "-O0"
)
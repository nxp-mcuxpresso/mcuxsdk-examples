
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS mdk iar
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DUSE_SDRAM\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DUSE_SDRAM\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor_sdram.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_flexspi_nor_sdram.scf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor_sdram.ld
)
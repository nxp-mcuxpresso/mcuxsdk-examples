# Add additional configuration
mcux_add_macro(
    CC "-DLWIP_ENET_FLEXIBLE_CONFIGURATION\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DENET_RXBUFF_NUM=14\
       -DENET_RXBD_NUM=9"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/lwip_examples/linker/lwip_tcm/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram_lwiptcm.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/lwip_examples/linker/lwip_tcm/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram_lwiptcm.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/lwip_examples/linker/lwip_tcm/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram_lwiptcm.ld
)

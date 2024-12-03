

# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
    CC "--dlib_config full"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)



# Remove additional configuration
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1020/MIMXRT1021/iar/MIMXRT1021xxxxx_flexspi_nor_sdram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1020/MIMXRT1021/arm/MIMXRT1021xxxxx_flexspi_nor_sdram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER devices/RT/RT1020/MIMXRT1021/gcc/MIMXRT1021xxxxx_flexspi_nor_sdram.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/aws_examples/common/linker/MIMXRT1021xxxxx_flexspi_nor_sdram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/aws_examples/common/linker/MIMXRT1021xxxxx_flexspi_nor_sdram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LINKER examples/_boards/${board}/aws_examples/common/linker/MIMXRT1021xxxxx_flexspi_nor_sdram.ld
)

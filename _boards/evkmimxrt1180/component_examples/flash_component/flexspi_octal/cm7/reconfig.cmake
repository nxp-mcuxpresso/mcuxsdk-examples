
mcux_add_macro(
    CC "-DCACHE_MAINTAIN=1\
       -DEXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK=kFLEXSPI_ReadSampleClkExternalInputFromDqsPad"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/iar/MIMXRT1189xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/arm/MIMXRT1189xxxxx_cm7_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/gcc/MIMXRT1189xxxxx_cm7_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/component_examples/flash_component/flexspi_octal/linker/MIMXRT1189xxxxx_cm7_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/component_examples/flash_component/flexspi_octal/linker/MIMXRT1189xxxxx_cm7_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/component_examples/flash_component/flexspi_octal/linker/MIMXRT1189xxxxx_cm7_flexspi_nor.ld
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000"
)

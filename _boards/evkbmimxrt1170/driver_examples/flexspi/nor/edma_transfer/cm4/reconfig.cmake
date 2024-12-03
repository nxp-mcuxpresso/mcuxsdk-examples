
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DENABLE_RAM_VECTOR_TABLE"
)

mcux_remove_macro(
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm4_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_cm4_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm4_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/edma_transfer/linker/MIMXRT1176xxxxx_cm4_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/edma_transfer/linker/MIMXRT1176xxxxx_cm4_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/edma_transfer/linker/MIMXRT1176xxxxx_cm4_flexspi_nor.ld
)

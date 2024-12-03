
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)

mcux_remove_macro(
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1050/MIMXRT1052/iar/MIMXRT1052xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1050/MIMXRT1052/arm/MIMXRT1052xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1050/MIMXRT1052/gcc/MIMXRT1052xxxxx_flexspi_nor.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/hyper_flash/polling_transfer/linker/MIMXRT1052xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/hyper_flash/polling_transfer/linker/MIMXRT1052xxxxx_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/hyper_flash/polling_transfer/linker/MIMXRT1052xxxxx_flexspi_nor.ld
)

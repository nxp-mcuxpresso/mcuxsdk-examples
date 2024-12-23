
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS flash_debug flash_release
    CC "-DENABLE_RAM_VECTOR_TABLE\
       -DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)
mcux_add_macro(
    TOOLCHAINS armgcc mdk
    TARGETS flash_debug flash_release
    CC "-DENABLE_RAM_VECTOR_TABLE"
)

mcux_add_linker_symbol(
    TARGETS flash_debug flash_release
    SYMBOLS "__ram_vector_table__=1"
)

mcux_add_linker_symbol(
    TARGETS flash_debug flash_release
    SYMBOLS "__stack_size__=0x800"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/octal/dma_transfer/iar/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/octal/dma_transfer/mdk/MIMXRT595Sxxxx_cm33_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/octal/dma_transfer/armgcc/MIMXRT595Sxxxx_cm33_flash.ld
)

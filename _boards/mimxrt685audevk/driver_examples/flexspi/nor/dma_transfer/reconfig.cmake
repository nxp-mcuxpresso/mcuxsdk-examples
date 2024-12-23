
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_macro(
    TARGETS flash_debug flash_release
    CC "-DENABLE_RAM_VECTOR_TABLE"
)

mcux_add_linker_symbol(
    TARGETS flash_debug flash_release
    SYMBOLS "__ram_vector_table__=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/dma_transfer/iar/MIMXRT685Sxxxx_cm33_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/dma_transfer/mdk/MIMXRT685Sxxxx_cm33_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/dma_transfer/armgcc/MIMXRT685Sxxxx_cm33_flash.ld
)

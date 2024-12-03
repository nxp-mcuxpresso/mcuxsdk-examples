mcux_add_macro(
    TOOLCHAINS iar mdk
    TARGETS flash_debug flash_release
    CC "-DENABLE_RAM_VECTOR_TABLE"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flash_debug flash_release
    CC "-DENABLE_RAM_VECTOR_TABLE\
       -DCOPY_RAM_FUNC=1"
)

mcux_add_iar_configuration(
    TARGETS flash_debug flash_release
    LD "--config_def=__ram_vector_table__=1"
)

mcux_add_mdk_configuration(
    TARGETS flash_debug flash_release
    LD "--predefine=\"-D__ram_vector_table__=1\""
)

mcux_add_armgcc_configuration(
    TARGETS flash_debug flash_release
    LD "-Xlinker --defsym=__ram_vector_table__=1"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/iar/RW612_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/arm/RW612_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/Wireless/RW/RW612/gcc/RW612_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/dma_transfer/iar/RW612_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/dma_transfer/mdk/RW612_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/driver_examples/flexspi/nor/dma_transfer/armgcc/RW612_flash.ld
)

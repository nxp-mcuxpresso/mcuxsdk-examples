
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/flexspi/octal/edma_transfer_unify
)

mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS flash_debug flash_release
    CC "-DXIP_EXTERNAL_FLASH=1\
       -DENABLE_RAM_VECTOR_TABLE"
)

mcux_add_iar_configuration(
    LD "--config_def=__ram_vector_table__=1\
        --config_def=__stack_size__=0x800"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__ram_vector_table__=1\
        -Xlinker --defsym=__stack_size__=0x800"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/i.MX/i.MX8ULP/MIMX8UD7/iar/MIMX8UD7xxxxx_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/i.MX/i.MX8ULP/MIMX8UD7/gcc/MIMX8UD7xxxxx_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/driver_examples/flexspi/octal/edma_transfer_unify/linker/MIMX8UD7xxxxx_cm33_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/driver_examples/flexspi/octal/edma_transfer_unify/linker/MIMX8UD7xxxxx_cm33_flexspi_nor.ld
)

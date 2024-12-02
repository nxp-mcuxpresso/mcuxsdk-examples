
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexspi/common/nor_flash_macronix.c
            examples/_boards/evkmimx8ulp/driver_examples/flexspi/octal/edma_transfer_unify/./app.h
            examples/_boards/evkmimx8ulp/driver_examples/flexspi/octal/edma_transfer_unify/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/flexspi/octal/edma_transfer_unify/.
             examples/driver_examples/flexspi/octal/edma_transfer_unify
             examples/_boards/evkmimx8ulp
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

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/iar/MIMX8UD5xxxxx_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/gcc/MIMX8UD5xxxxx_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/evkmimx8ulp/driver_examples/flexspi/octal/edma_transfer_unify/linker/MIMX8UD7xxxxx_cm33_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/evkmimx8ulp/driver_examples/flexspi/octal/edma_transfer_unify/linker/MIMX8UD7xxxxx_cm33_flexspi_nor.ld
)

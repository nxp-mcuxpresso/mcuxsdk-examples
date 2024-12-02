

# Add additional configuration
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1064/MIMXRT1064/iar/MIMXRT1064xxxxx_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1064/MIMXRT1064/gcc/MIMXRT1064xxxxx_flexspi_nor.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1064/MIMXRT1064/arm/MIMXRT1064xxxxx_flexspi_nor.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1064xxxxx_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1064xxxxx_flexspi_nor.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1064xxxxx_flexspi_nor.scf
)

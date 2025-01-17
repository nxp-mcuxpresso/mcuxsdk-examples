


# Add additional configuration
mcux_add_macro(
    CC "-DENCRYPTED_XIP_BEE"
)
mcux_add_macro(
    CC "-DSKIP_SYSCLK_INIT\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1020/MIMXRT1021/iar/MIMXRT1021xxxxx_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1020/MIMXRT1021/gcc/MIMXRT1021xxxxx_flexspi_nor.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1020/MIMXRT1021/arm/MIMXRT1021xxxxx_flexspi_nor.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1021xxxxx_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1021xxxxx_flexspi_nor.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1021xxxxx_flexspi_nor.scf
)

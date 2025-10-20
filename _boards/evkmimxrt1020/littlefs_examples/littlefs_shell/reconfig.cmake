mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
       -DSDK_DEBUGCONSOLE_UART\
       -DSKIP_SYSCLK_INIT\
       -DXIP_BOOT_HEADER_DCD_ENABLE=1\
       -DXIP_EXTERNAL_FLASH=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS hyperram_release release sdram_release flexspi_nor_release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1020/MIMXRT1021/iar/MIMXRT1021xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1020/MIMXRT1021/arm/MIMXRT1021xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1020/MIMXRT1021/gcc/MIMXRT1021xxxxx_flexspi_nor.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1021xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1021xxxxx_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1021xxxxx_flexspi_nor.ld
)

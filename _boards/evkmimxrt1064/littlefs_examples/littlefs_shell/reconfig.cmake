# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1\
        -DSDK_DEBUGCONSOLE_UART\
        -DSKIP_SYSCLK_INIT\
        -DXIP_BOOT_HEADER_DCD_ENABLE=1\
        -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS flexspi_nor_release release sdram_release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)

mcux_remove_macro(
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1064/MIMXRT1064/iar/MIMXRT1064xxxxx_flexspi_nor.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1064/MIMXRT1064/arm/MIMXRT1064xxxxx_flexspi_nor.scf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1064/MIMXRT1064/gcc/MIMXRT1064xxxxx_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1064xxxxx_flexspi_nor.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1064xxxxx_flexspi_nor.scf
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/linker/MIMXRT1064xxxxx_flexspi_nor.ld
)

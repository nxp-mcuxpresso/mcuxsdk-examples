# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXW/MCXW727C/iar/mcxw727_cm33_core0_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXW/MCXW727C/gcc/mcxw727_cm33_core0_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXW/MCXW727C/iar/mcxw727_cm33_core0_flash_rom_boot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXW/MCXW727C/gcc/mcxw727_cm33_core0_flash_rom_boot.ld
)

mcux_add_macro(
    CC "-DDEMO_PD_MODE_NOT_OK=1"
    AS "-DECC_RAM_STCM8_INIT"
)

# Add additional configuration
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_macro(
    CC "PSA_WANT_ALG_GCM\
        PSA_WANT_ALG_SHA_1"
)
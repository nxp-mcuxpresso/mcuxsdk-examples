# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/trustzone_examples/linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.ld
)

mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/trustzone_examples/linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.scf
)

mcux_add_iar_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/trustzone_examples/linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.icf
)

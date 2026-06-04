# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

# Replace the default device linker scripts with the ones customised for this
# example (larger flash region starts after the littlefs volume).

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/${core_id}/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/${core_id}/mdk/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/littlefs_examples/littlefs_shell/${core_id}/armgcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

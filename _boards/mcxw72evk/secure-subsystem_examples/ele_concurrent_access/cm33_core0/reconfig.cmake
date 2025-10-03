#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)

# Use our custom example-specific linkers, so remove the defaults and add ours
mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/MCX/MCXW/MCXW727C/gcc/mcxw727_cm33_core0_flash.ld
)
mcux_remove_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/MCX/MCXW/MCXW727C/iar/mcxw727_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/MCX/MCXW/MCXW727C/arm/mcxw727_cm33_core0_flash.scf
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access
    LINKER linker/gcc/concurrent_access_cm33_core0_flash.ld
)
mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access
    LINKER linker/iar/concurrent_access_cm33_core0_flash.icf
)
mcux_add_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access
    LINKER linker/arm/concurrent_access_cm33_core0_flash.scf
)

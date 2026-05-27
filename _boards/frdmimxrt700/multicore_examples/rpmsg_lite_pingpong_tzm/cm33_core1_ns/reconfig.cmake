#
# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_ns/hardware_init.c
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_ns/app.h
)
mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_ns)


# Remove no_se from IAR FLags
mcux_remove_iar_configuration(
        AS "--cpu=cortex-m33.no_se"
        CC "--cpu=cortex-m33.no_se"
        CX "--cpu=cortex-m33.no_se"
        LD "--cpu=cortex-m33.no_se"
)

# And then this will add Trust Zone Enable clicky button checked in IAR GUI Project
mcux_add_iar_configuration(
        AS "--cpu=cortex-m33"
        CC "--cpu=cortex-m33"
        CX "--cpu=cortex-m33"
        LD "--cpu=cortex-m33"
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_mdk_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_ns.icf
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_ns.ld
)

mcux_add_mdk_linker_script(
        TARGETS debug release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_ns.scf
)

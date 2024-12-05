# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/trustzone_examples/hello_world/hello_world_s/tzm_config.c
            ${board_root}/${board}/trustzone_examples/hello_world/hello_world_s/tzm_config.h
            ${board_root}/kw45b41zevk/trustzone_examples/hello_world/hello_world_s/hardware_init.c
            ${board_root}/kw45b41zevk/trustzone_examples/hello_world/hello_world_s/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/trustzone_examples/hello_world/hello_world_s
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.ld
)

mcux_add_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.scf
)

mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.icf
)
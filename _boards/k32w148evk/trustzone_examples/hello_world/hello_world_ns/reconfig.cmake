# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.ld
)

mcux_add_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.scf
)

mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.icf
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/trustzone_examples/hello_world/hello_world_ns/hardware_init.c
            examples/_boards/kw45b41zevk/trustzone_examples/hello_world/hello_world_ns/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/trustzone_examples/hello_world/hello_world_ns
)

mcux_add_macro(
    AS "-DBYPASS_ECC_RAM_INIT"
    CC "-DBYPASS_ECC_RAM_INIT"
)
# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_source(
    SOURCES hello_world_debug_ram_debug.ini
            hello_world_debug_ram_release.ini
            hello_world_download_ram_debug.ini
            hello_world_download_ram_release.ini
            MIMXRT1180-EVK_TCM.mex
            tzm_config.c
            tzm_config.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.ld
)

mcux_add_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.scf
)

mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.icf
)

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_s.ld
)

mcux_add_iar_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_s.icf
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS debug release
    CC "-DDEMO_CODE_START_NS=0x0FFF0000U"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DDEMO_CODE_START_NS=0x2880B000U"
)
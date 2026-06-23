# Copyright 2024-2025 NXP
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_ns.ld
)

mcux_add_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_ns.scf
)

mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_ns.icf
)

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_ns.ld
)

mcux_add_mdk_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_ns.scf
)

mcux_add_iar_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_ns.icf
)

mcux_remove_mdk_configuration(
    LD "--fpu=FPv5-SP"
)

mcux_add_mdk_configuration(
        AS "-mfpu=none -mfloat-abi=soft"
        CC "-mfpu=none -mfloat-abi=soft"
        CX "-mfpu=none -mfloat-abi=soft"
        LD "--fpu=softvfp --load_addr_map_info --library_type=microlib"
)

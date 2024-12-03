
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/wifi_examples/common/hardware_init.c
        examples/_boards/${board}/wifi_examples/common/app.h
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/_boards/${board}/wifi_examples/common
)

mcux_add_mdk_configuration(
    TARGETS flash_debug flash_release
    LD "--no_remove"
)

mcux_remove_mdk_configuration(
    TARGETS flash_debug flash_release
    LD "--remove"
)

mcux_add_iar_configuration(
    CX "--no_clustering"
    TARGETS flash_release
    CC "--no_inline"
)


# Rename wifi linker files on RW612

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW612_wifi.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW612_wifi.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW612_wifi.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/RW610_flash.scf
)


# Remove all default linker files
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)
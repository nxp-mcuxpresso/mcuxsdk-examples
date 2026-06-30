
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/wifi_bt_config.c
        ${board_root}/${board}/wifi_bt_config.h
        ${board_root}/${board}/sdmmc_config.c
        ${board_root}/${board}/sdmmc_config.h
        ${board_root}/${board}/wifi_examples/common/hardware_init.c
        ${board_root}/${board}/wifi_examples/common/app.h
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        ${board_root}/${board}
        ${board_root}/${board}/wifi_examples/common
)

mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ"
)

mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
)
mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)

mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1\
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)

# Remove all default linker files
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.scf
)

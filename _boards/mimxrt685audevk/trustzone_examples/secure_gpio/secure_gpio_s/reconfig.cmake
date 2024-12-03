# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_debug_debug.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_debug_flash_debug.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_debug_flash_release.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_debug_release.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_download_debug.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_download_flash_debug.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_download_flash_release.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/secure_gpio_download_release.ini
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/tzm_config.c
            examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_s/tzm_config.h
            examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
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
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.ld
)

mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.scf
)

mcux_add_iar_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.icf
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS debug release
    CC "-DDEMO_CODE_START_NS=786432"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS flash_debug flash_release
    CC "-DDEMO_CODE_START_NS=135266304"
)

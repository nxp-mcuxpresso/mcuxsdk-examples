
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/wifi_bt_config.c
        examples/_boards/${board}/wifi_bt_config.h
        examples/_boards/${board}/sdmmc_config.c
        examples/_boards/${board}/sdmmc_config.h
        examples/_boards/${board}/wifi_examples/common/hardware_init.c
        examples/_boards/${board}/wifi_examples/common/app.h
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/_boards/${board}
        examples/_boards/${board}/wifi_examples/common
)

# Reconfigure this example for MIMXRT1020 board
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/evkmimxrt1020/wifi_examples/wifi_setup/wifi_config.h
        examples/_boards/evkmimxrt1020/wifi_examples/wifi_setup/lwipopts.h
        examples/_boards/evkmimxrt1020/wifi_examples/wifi_setup/FreeRTOSConfig.h

)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/_boards/evkmimxrt1020/wifi_examples/wifi_setup
)

mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/wifi_examples/wifi_setup/wifi_config/wifi_config.h
        examples/wifi_examples/wifi_setup/lwipopts/lwipopts.h
        examples/wifi_examples/wifi_setup/FreeRTOSConfig.h
)

mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/wifi_examples/wifi_setup/wifi_config
        examples/wifi_examples/wifi_setup/lwipopts
        examples/wifi_examples/wifi_setup
)


# Remove all default linker files
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.scf
)
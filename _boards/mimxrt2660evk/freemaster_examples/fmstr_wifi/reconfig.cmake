# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x400"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400"
)

mcux_add_mdk_configuration(
    CC "-mfloat-abi=hard"
)

mcux_add_iar_configuration(
    CC "--dlib_config full --no_inline"
    CX "--no_clustering"
    LD "--semihosting"
)

mcux_add_macro(
    CC "BOARD_USE_PCAL6524=1 BOARD_USE_PCA9555=1 SDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

# Use board specific implementation
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
      ${board_root}/${board}/sdmmc_config.c
      ${board_root}/${board}/sdmmc_config.h
      ${board_root}/${board}/wifi_bt_config.c
      ${board_root}/${board}/wifi_bt_config.h
)

# Remove default linker scripts
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)

# Add wifi-specific linker scripts
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
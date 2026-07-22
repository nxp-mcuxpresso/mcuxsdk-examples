
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# CMakeLists.txt adds pin_mux.c/h from wifi_examples/common/ which does not exist
# on frdmimxrt700. Remove them here; pin_mux is provided via pinmux_board_core_folder prjseg.
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/wifi_examples/common/pin_mux.c
        ${board_root}/${board}/wifi_examples/common/pin_mux.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/wifi_examples/common/cm33_core0/hardware_init.c
        ${board_root}/${board}/wifi_examples/common/cm33_core0/app.h
        ${board_root}/${board}/sdmmc_config.c
        ${board_root}/${board}/sdmmc_config.h
        ${board_root}/${board}/wifi_bt_config.c
        ${board_root}/${board}/wifi_bt_config.h
        ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        ${board_root}/${board}/wifi_examples/common
        ${board_root}/${board}/wifi_examples/common/cm33_core0
)

mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

# Remove default RAM linker scripts (debug/release targets use RAM linker by default on RT700)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

# Remove default flash linker scripts
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)
# Note: webconfig linker scripts and stack/heap defsyms are already added by CMakeLists.txt

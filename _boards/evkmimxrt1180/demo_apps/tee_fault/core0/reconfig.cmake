#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug
    INCLUDES ${board_root}/evkmimxrt1180/demo_apps/tee_fault/core0/../core1/armgcc/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release
    INCLUDES ${board_root}/evkmimxrt1180/demo_apps/tee_fault/core0/../core1/armgcc/release
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS flexspi_nor_debug
    INCLUDES ${board_root}/evkmimxrt1180/demo_apps/tee_fault/core0/../core1/mdk/debug
)
mcux_add_asm_include(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS mdk
    TARGETS flexspi_nor_release
    INCLUDES ${board_root}/evkmimxrt1180/demo_apps/tee_fault/core0/../core1/mdk/release
)

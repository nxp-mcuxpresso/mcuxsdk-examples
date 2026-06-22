#
# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mu_interrupt_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/mu/interrupt/)

include(${SdkRootDirPath}/CMakeLists.txt)

# Allow a board to override per-core settings for the secondary core (for
# example, swap the default flash linker for a RAM linker). Included after the
# board CMakeLists.txt has already registered its default linker so the override
# can take effect. Boards that need no override may omit reconfig.cmake or
# provide a comment-only placeholder.
include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/mu/interrupt/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES mu_interrupt_core1.c
)

mcux_add_include(
    INCLUDES .
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_hseb_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ele_hseb/ele_hseb_demo_secondary_core)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/${board_root}/${board}/ele_hseb/ele_hseb_demo_secondary_core/${core_id}/reconfig.cmake OPTIONAL)

# We use the original demo; there should be no change in behavior
mcux_add_source(
    SOURCES ../../ele_hseb_demo/ele_hseb_demo.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_configuration(
    CC "-D__SEMIHOST_HARDFAULT_DISABLE=1"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

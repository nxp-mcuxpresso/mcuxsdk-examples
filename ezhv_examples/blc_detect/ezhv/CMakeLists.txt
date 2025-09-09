# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_blc_detect_ezhv LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ezhv_examples/blc_detect)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES blc_detect_ezhv.c
)

include(${SdkRootDirPath}/${board_root}/${board}/ezhv_examples/blc_detect/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/ezhv_image.bin TOOLCHAINS riscvllvm)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_switch_primary LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_mode_switch/)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../power_mode_switch_secondary/iar/core1_image.bin,_core1_image,__core1_image,4 \
        --keep=_core1_image"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib --keep=*(*core1_code)"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../power_mode_switch_secondary/${CONFIG_TOOLCHAIN}/
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_switch/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

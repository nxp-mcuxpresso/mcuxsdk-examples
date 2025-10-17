# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_blc_detect_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ezhv_examples/blc_detect)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES blc_detect_primary_core.c
)

mcux_add_include(
    BASE_PATH ${APPLICATION_BINARY_DIR}/../ezhv_blc_detect_ezhv/${SB_CONFIG_ezhv_toolchain}
    INCLUDES ./
    TOOLCHAINS mdk armgcc
)

include(${SdkRootDirPath}/${board_root}/${board}/ezhv_examples/blc_detect/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../ezhv_blc_detect_ezhv/${SB_CONFIG_ezhv_toolchain}/ezhv_image.bin,_ezhv_image,__ezhv_image,4 \
        --keep _ezhv_image"
)

mcux_add_mdk_configuration(
    LD "--keep=*(*ezhv_code)"
)

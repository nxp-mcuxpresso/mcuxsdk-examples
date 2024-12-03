#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mu_polling_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/mu/polling/)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES mu_polling_core0.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_iar_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
        LD "--image_input=${APPLICATION_BINARY_DIR}/../mu_polling_secondary_core/iar/core1_image.bin,_core1_image,__core1_image,4 \
            --keep _core1_image"
)

mcux_add_armgcc_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_mdk_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
        LD "--library_type=standardlib --keep=*(*core1_code)"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../mu_polling_secondary_core/${CONFIG_TOOLCHAIN}/
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/mu/polling/${core_id}/reconfig.cmake OPTIONAL)

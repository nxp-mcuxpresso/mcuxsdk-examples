#
# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mu_interrupt_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/mu/interrupt/)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES mu_interrupt_core0.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

# The secondary core (M7_1) image is embedded inside this primary flash image
# and linked to a dedicated flash region (0x00600000) so that flashing the
# primary also programs the secondary; M7_1 then runs in place from flash. The
# secondary is NOT copied to RAM, so CORE1_IMAGE_COPY_TO_RAM is not defined.
#
# IAR: --image_input embeds the secondary binary into the __core1_image section,
# which the core0 flash linker pins at 0x00600000.
mcux_add_iar_configuration(
        LD "--image_input=${APPLICATION_BINARY_DIR}/../mu_interrupt_secondary_core/iar/core1_image.bin,_core1_image,__core1_image,4 \
            --keep _core1_image"
)

mcux_add_mdk_configuration(
        LD "--library_type=standardlib --keep=*(*core1_code)"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../mu_interrupt_secondary_core/${CONFIG_TOOLCHAIN}/
)

mcux_add_codewarrior_lib_path(
    LIB_PATH "${APPLICATION_BINARY_DIR}/../mu_interrupt_secondary_core"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/mu/interrupt/${core_id}/reconfig.cmake OPTIONAL)

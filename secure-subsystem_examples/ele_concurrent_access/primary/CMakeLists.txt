#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_concurrent_access_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main.c
            ../common_definitions.h
)

mcux_add_include(
    INCLUDES .
    INCLUDES ..
)

# Make sure rpmsg shared memory is enabled for intercore communication
# and we bump the stack size a little
mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
            "__stack_size__=0x800"
)

# Use our custom example-specific linkers, so remove the defaults and add ours
mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/Wireless/KW/KW47B42ZB7/gcc/kw47b42zb_cm33_core0_flash.ld
)
mcux_remove_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/Wireless/KW/KW47B42ZB7/iar/kw47b42zb_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/Wireless/KW/KW47B42ZB7/arm/kw47b42zb_cm33_core0_flash.scf
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access
    LINKER linker/gcc/concurrent_access_cm33_core0_flash.ld
)
mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access
    LINKER linker/iar/concurrent_access_cm33_core0_flash.icf
)
mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access
    LINKER linker/arm/concurrent_access_cm33_core0_flash.scf
)

mcux_add_configuration(
    CC "-DMCMGR_HANDLE_EXCEPTIONS=1 -D__SEMIHOST_HARDFAULT_DISABLE=1"
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../ele_concurrent_access_secondary_core/iar/core1_image.bin,_core1_image,__core1_image,4 \
        --keep _core1_image"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib --keep=*(*core1_code)"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../ele_concurrent_access_secondary_core/${CONFIG_TOOLCHAIN}/
)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_tzm_primary_core_s LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/s)

include(${SdkRootDirPath}/CMakeLists.txt)

# This will point to: ${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/reconfig.cmake
# where common board multicore properties are set.
include(${CMAKE_CURRENT_LIST_DIR}/../../config.cmake)

# Include reconfigure for Secure Project
include(${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/s/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main.c
            ../veneer_table.c
            ../veneer_table.h
)

mcux_add_include(
    INCLUDES .
             ../
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/trustzone_examples/tzm_api/tzm_api.c
            examples/trustzone_examples/tzm_api/tzm_api.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/trustzone_examples/tzm_api/
)

mcux_add_configuration(
        CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __heap_size__=0x2000 \
             __use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_add_iar_configuration(
    CC "--cmse"
    LD "--image_input=${APPLICATION_BINARY_DIR}/../rpmsg_lite_pingpong_tzm_secondary_core_s/iar/core1_image.bin,_core1_image,__core1_image,4 \
        --keep _core1_image \
        --import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_s_CMSE_lib.o"
)

# Create output directory for armgcc to put library in.
mcux_add_custom_command(
    BUILD_EVENT PRE_BUILD
    TOOLCHAINS armgcc
    BUILD_COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}
)

mcux_add_armgcc_configuration(
    CC "-mcmse"
    LD "-Wl,--cmse-implib \
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_s_CMSE_lib.o"
)

mcux_add_mdk_configuration(
    CC "-mcmse"
    LD "--library_type=standardlib \
       --import-cmse-lib-out=\"${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_s_CMSE_lib.o\""
)

mcux_add_include(
        TOOLCHAINS mdk armgcc
        BASE_PATH ${APPLICATION_BINARY_DIR}
        INCLUDES ../rpmsg_lite_pingpong_tzm_secondary_core_s/${CONFIG_TOOLCHAIN}/
)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tfm_demo_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/tfm_examples/tfm_demo/tfm_demo_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
# Include reconfigure for Non Secure Project
include(${SdkRootDirPath}/examples/_boards/${board}/tfm_examples/tfm_demo/tfm_demo_ns/reconfig.cmake OPTIONAL)

#mcux_add_source(
#    SOURCES tfm_demo_ns.c
#             ../tfm_demo_s/veneer_table.h
#)

mcux_add_macro(
     CC "-DOS_DYNAMIC_MEM_SIZE=7168\
       "
)

mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable -Wno-unused-but-set-variable -Wno-return-type -Wno-maybe-uninitialized"
)

mcux_add_iar_configuration(
    CC "--diag_suppress Pa082,Pa050,Pa084,Pa205,Pe128,Pe186,Pe546,Pe940,Pe550,Pa167,Pe161"
    CX "--diag_suppress Pa082,Pa050,Pa084,Pa205,Pe128,Pe186,Pe546,Pe940,Pe550,Pa167,Pe161"
)

#add pre build commands for linker file
mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS armgcc
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -E -P -xc -I${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns.ld -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns_pre.ld
)

mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS iar
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${CC} -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns.icf --silent --preprocess=ns ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns_pre.icf
)
mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS mdk
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${CC} --target=arm-arm-none-eabi -march=armv8-m.main -E -P -xc -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns.sct -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns_pre.sct

)

#----------------------------------------------
# Linker file configurations for all toolchains
#----------------------------------------------

# remove predefined linker and use trustzone specific one
mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_remove_mdk_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

# remove predefined linker and use trustzone specific one
mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)
mcux_remove_mdk_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)

# Add generic Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns_pre.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns_pre.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns_pre.sct
)

# add tfm demo secure project
mcux_add_include(
    INCLUDES ../tfm_demo_s
)

#add tfm_demo_s lib
mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../tfm_demo_s/${CONFIG_TOOLCHAIN}/tfm_demo_s_CMSE_lib.o"
    TOOLCHAINS armgcc mdk iar
)

#add step to convert the outout to *.bin.
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${MCUX_SDK_PROJECT_NAME}.bin)

# Create output directory to put output files in.
mcux_add_custom_command(
        BUILD_EVENT PRE_BUILD
        BUILD_COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}
)
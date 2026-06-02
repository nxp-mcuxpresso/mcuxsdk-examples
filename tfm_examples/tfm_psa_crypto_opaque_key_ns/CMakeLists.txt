# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tfm_psa_crypto_opaque_key_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/tfm_examples/tfm_psa_crypto_opaque_key/tfm_psa_crypto_opaque_key_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
# Include reconfigure for Non Secure Project
include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/tfm_psa_crypto_opaque_key/tfm_psa_crypto_opaque_key_ns/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/tfm_examples/tfm_psa_crypto_opaque_key_ns/psa_crypto_opaque_key.c
            examples/tfm_examples/tfm_psa_crypto_opaque_key_ns/mcux_mbedtls_tfm_psa_crypto_key_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/tfm_examples/tfm_psa_crypto_opaque_key_ns
)



mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_tfm_psa_crypto_key_config.h\\\""
)


mcux_add_macro(
     CC "-DOS_DYNAMIC_MEM_SIZE=7168\
       "
)

#mcux_add_macro(
#    CC "MBEDTLS_USER_CONFIG_FILE=\\\"mcux_mbedtls_psa_crypto_opaque_config.h\\\"\
#        PRINTF_ADVANCED_ENABLE=1"
#)

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
        BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -D${device}${core_id_suffix_name}_SERIES -E -P -xc -I${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns.ld -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns_pre.ld
)

mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS iar
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${CC} -D${device}${core_id_suffix_name}_SERIES -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns.icf --silent --preprocess=ns ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns_pre.icf
)
mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS mdk
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${CC} -D${device}${core_id_suffix_name}_SERIES --target=arm-arm-none-eabi -march=armv8-m.main -E -P -xc -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns.sct -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns_pre.sct

)

#----------------------------------------------
# Linker file configurations for all toolchains
#----------------------------------------------

# remove predefined linker and use trustzone specific one
mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_remove_mdk_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

# remove predefined linker and use trustzone specific one
mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)
mcux_remove_mdk_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
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
    INCLUDES ../tfm_psa_crypto_opaque_key_s
)

#add tfm_psa_crypto_opaque_key_s lib
mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../tfm_psa_crypto_opaque_key_s/${CONFIG_TOOLCHAIN}/tfm_psa_crypto_opaque_key_s${core_id_suffix_name}_CMSE_lib.o"
    TOOLCHAINS armgcc mdk iar
    GENERATED TRUE
)

#add step to convert the outout to *.bin.
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

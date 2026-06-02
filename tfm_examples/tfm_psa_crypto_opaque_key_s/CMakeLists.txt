#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tfm_psa_crypto_opaque_key_s LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/tfm_examples/tfm_psa_crypto_opaque_key/tfm_psa_crypto_opaque_key_s)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/tfm_psa_crypto_opaque_key/tfm_psa_crypto_opaque_key_s/reconfig.cmake OPTIONAL)

#add cc-define for all tfm_psa_crypto_opaque_key_s examples 
mcux_add_macro(
     CC "-DTFM_SPM_LOG_LEVEL=TFM_SPM_LOG_LEVEL_INFO\
       -DTFM_SP_LOG_RAW_ENABLED\
       -DTFM_PARTITION_LOG_LEVEL=TFM_PARTITION_LOG_LEVEL_INFO\
       -DPLATFORM_NO_FLASH\
       -DDAUTH_CHIP_DEFAULT\
       -D__SEMIHOST_HARDFAULT_DISABLE\
       -DENABLE_HEAP\
       "
)

#--------------------------
# add iar configurations
#--------------------------
mcux_add_iar_configuration(
    CC "--cmse --diag_suppress Pa039,Pa082,Pa050,Pa205,Pa084,Pe546,Pe550,Pe940,Pe188,Pe186,Pe177,Pe111,Pe174,Pe128,Pe161,Pe068,Pe223,Pa181"
    CX "--cmse --diag_suppress Pa039,Pa082,Pa050,Pa205,Pa084,Pe546,Pe550,Pe940,Pe188,Pe186,Pe177,Pe111,Pe174,Pe128,Pe161,Pe068,Pe223,Pa181"
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_psa_crypto_opaque_key_s${core_id_suffix_name}_CMSE_lib.o"
)

#pre build commands for linker file for iar
mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS iar
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${CC} --cpu=Cortex-M33 -D${device}${core_id_suffix_name}_SERIES -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP -D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/iar/tfm_common_s.icf --silent --preprocess=ns ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/iar/tfm_common_s_pre.icf
)

#--------------------------
# add armgcc configurations
#--------------------------
mcux_add_armgcc_configuration(
    CC "-mcmse -Wno-unused-variable -Wno-unused-value -Wno-unused-function -Wno-unused-but-set-variable -Wno-return-type -Wno-bool-operation -Wno-maybe-uninitialized -Wno-format"
    CX "-mcmse"
    LD "-Wl,--cmse-implib \
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_psa_crypto_opaque_key_s${core_id_suffix_name}_CMSE_lib.o"
)

#pre build commands for linker file for armgcc
mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS armgcc
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -D${device}${core_id_suffix_name}_SERIES -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP -D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE -E -P -xc -I${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition -I${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s.ld -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld
)

#--------------------------
# add mdk configurations
#--------------------------
mcux_add_mdk_configuration(
    CC "-mcmse"
    CX "-mcmse"
    LD "--diag_suppress=6329,6312,1296 --import-cmse-lib-out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/tfm_psa_crypto_opaque_key_s${core_id_suffix_name}_CMSE_lib.o"
)

#pre build commands for linker file for mdk
mcux_add_custom_command(
        TARGETS debug release
        TOOLCHAINS mdk
        BUILD_EVENT  PRE_BUILD
        BUILD_COMMAND ${CC} -D${device}${core_id_suffix_name}_SERIES -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP -D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE --target=arm-arm-none-eabi
        -march=armv8-m.main -E -P -xc -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
        -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armclang/tfm_common_s.sct
        -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armclang/tfm_common_s_pre.sct

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

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/target/nxp/common/iar/tfm_common_s_pre.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/target/nxp/common/armclang/tfm_common_s_pre.sct
)


# add tfm_spm, mbedtls, crypto and ps secure project
mcux_add_include(
    INCLUDES 
	../tfm_s_crypto_clients
)

#add tfm_spm lib
mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS 
	     "../tfm_s_crypto_clients/${CONFIG_TOOLCHAIN}/tfm_s_crypto_clients${core_id_suffix_name}.lib"
    TOOLCHAINS iar armgcc mdk
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

# Create output directory to put library in.
file(MAKE_DIRECTORY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN})

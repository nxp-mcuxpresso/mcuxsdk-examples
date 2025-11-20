# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_import_blob_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/el2go_examples/el2go_import_blob/el2go_import_blob_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/el2go_import_blob/el2go_import_blob_ns/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob/*.c
            middleware/nxp_iot_agent/port/el2go_mbedtls_user_config_bare_metal.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob
             middleware/nxp_iot_agent/port
)

mcux_add_macro(
    CC "MBEDTLS_USER_CONFIG_FILE=\\\"el2go_mbedtls_user_config_bare_metal.h\\\"\
        PRINTF_ADVANCED_ENABLE=1"
)

mcux_add_iar_configuration(
    CC "--no_inline\
        --diag_suppress=Pa084,Pe188,Pe546"
)

mcux_add_mdk_configuration(
    CC "-Wno-typedef-redefinition -Wno-parentheses-equality"
    LD "--library_type=microlib"
)
 
# TF-M linker file preprocessing
mcux_add_custom_command(
    TOOLCHAINS armgcc
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns.ld
    -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns_pre.ld
)

mcux_add_custom_command(
    TOOLCHAINS iar
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC}
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns.icf
    --silent --preprocess=ns ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns_pre.icf
)

mcux_add_custom_command(
    TOOLCHAINS mdk
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC} --target=arm-arm-none-eabi -march=armv8-m.main -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns.sct
    -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns_pre.sct
)

# Add preprocessed TF-M linker files
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

# Add TF-M S import library
mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../el2go_import_blob_s/${CONFIG_TOOLCHAIN}/el2go_import_blob_s${core_id_suffix_name}_CMSE_lib.o"
    TOOLCHAINS armgcc iar mdk
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
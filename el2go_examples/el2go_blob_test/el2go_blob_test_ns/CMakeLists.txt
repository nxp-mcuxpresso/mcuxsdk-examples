cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_blob_test_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/el2go_examples/el2go_blob_test/el2go_blob_test_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/el2go_blob_test/el2go_blob_test_ns/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/nxp_iot_agent/tst/el2go_blob_test/inc/*.h
            middleware/nxp_iot_agent/tst/el2go_blob_test/src/*.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/nxp_iot_agent/tst/el2go_blob_test/inc
)

mcux_add_macro(
    CC "PRINTF_ADVANCED_ENABLE=1"
)

mcux_add_iar_configuration(
    CC "--diag_suppress=Pe546"
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
    LIBS "../el2go_blob_test_s/${CONFIG_TOOLCHAIN}/el2go_blob_test_s${core_id_suffix_name}_CMSE_lib.o"
    TOOLCHAINS armgcc iar mdk
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
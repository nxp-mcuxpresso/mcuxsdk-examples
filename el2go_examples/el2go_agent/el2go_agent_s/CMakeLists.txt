cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_agent_s LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/el2go_examples/el2go_agent/el2go_agent_s)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/el2go_agent/el2go_agent_s/reconfig.cmake OPTIONAL)

mcux_add_macro(
    CC "TFM_SPM_LOG_LEVEL=TFM_SPM_LOG_LEVEL_INFO\
        TFM_PARTITION_LOG_LEVEL=TFM_PARTITION_LOG_LEVEL_INFO\
        TFM_SP_LOG_RAW_ENABLED\
        DAUTH_CHIP_DEFAULT\
        ENABLE_HEAP\
        TFM_WIFI_FLASH_REGION\
        TFM_EL2GO_DATA_IMPORT_REGION"
)

# Create directory for CMSE import library to stay compatible with IDE builds
file(MAKE_DIRECTORY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN})

# CMSE config
mcux_add_armgcc_configuration(
    CC "-mcmse\
        -Wno-unused-but-set-variable -Wno-unused-value"
    LD "-Wl,--cmse-implib\
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/el2go_agent_s_CMSE_lib.o"
)

mcux_add_iar_configuration(
    CC "--cmse\
        --diag_suppress=Pa039,Pe068,Pa084,Pe111,Pe174,Pa181,Pe188,Pe223,Pe546,Pe550,Pe940"
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/el2go_agent_s_CMSE_lib.o"
)

mcux_add_mdk_configuration(
    CC "-mcmse"
    LD "--import-cmse-lib-out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/el2go_agent_s_CMSE_lib.o\
        --diag_suppress=6329"
)

# TF-M linker file preprocessing
mcux_set_list(
    TFM_LINKER_DEFINES_WIFI_IMPORT "-D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP -DTFM_WIFI_FLASH_REGION -DTFM_EL2GO_DATA_IMPORT_REGION"
)

mcux_add_custom_command(
    TOOLCHAINS armgcc
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc ${TFM_LINKER_DEFINES_WIFI_IMPORT} -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s.ld
    -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armgcc/tfm_common_s_pre.ld
)

mcux_add_custom_command(
    TOOLCHAINS iar
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC} ${TFM_LINKER_DEFINES_WIFI_IMPORT} --cpu=Cortex-M33
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/iar/tfm_common_s.icf
    --silent --preprocess=ns ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/iar/tfm_common_s_pre.icf
)

mcux_add_custom_command(
    TOOLCHAINS mdk
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC} ${TFM_LINKER_DEFINES_WIFI_IMPORT} --target=arm-arm-none-eabi -march=armv8-m.main -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armclang/tfm_common_s.sct
    -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/common/armclang/tfm_common_s_pre.sct
)

# Add preprocessed TF-M linker files
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

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
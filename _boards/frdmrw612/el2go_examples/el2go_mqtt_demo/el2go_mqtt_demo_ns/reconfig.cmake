mcux_add_macro(
    CC "USE_OCOTP_DRIVER_IN_LOAD_SERVICE"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    PREINCLUDE TRUE
    SOURCES examples/_boards/${board}/el2go_examples/common/wifi/app_config.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/el2go_examples/common/wifi/lwipopts.h
            examples/el2go_examples/common/wifi/lwippools.h
            examples/el2go_examples/common/wifi/wifi_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/el2go_examples/common/wifi
)

# TF-M linker file preprocessing
mcux_add_custom_command(
    TOOLCHAINS armgcc
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns.ld
    -o ${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns_pre.ld
)

mcux_add_custom_command(
    TOOLCHAINS iar
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC}
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns.icf
    --silent --preprocess=ns ${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns_pre.icf
)

mcux_add_custom_command(
    TOOLCHAINS mdk
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC} --target=arm-arm-none-eabi -march=armv8-m.main -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns.sct
    -o ${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns_pre.sct
)

# Add preprocessed TF-M linker files
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns_pre.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns_pre.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/el2go_examples/common/linker/tfm_common_ns_pre.sct
)
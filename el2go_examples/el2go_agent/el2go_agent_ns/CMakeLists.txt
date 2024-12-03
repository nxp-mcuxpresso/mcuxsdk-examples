cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_agent_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/el2go_examples/el2go_agent/el2go_agent_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/el2go_agent/el2go_agent_ns/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/el2go_examples/common/freertos/core_mqtt_config.h
            examples/el2go_examples/common/freertos/hooks.c
            examples/el2go_examples/common/wifi/lwipopts.h
            examples/el2go_examples/common/wifi/lwippools.h
            examples/el2go_examples/common/wifi/wifi_config.h
            middleware/nxp_iot_agent/ex/src/iot_agent_demo.c
            middleware/nxp_iot_agent/port/el2go_mbedtls_user_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/el2go_examples/common/freertos
             examples/el2go_examples/common/wifi
             middleware/nxp_iot_agent/port
)

mcux_add_macro(
    CC "AX_EMBEDDED=1\
        MBEDTLS_USER_CONFIG_FILE=\\\"el2go_mbedtls_user_config.h\\\"\
        NXP_IOT_AGENT_USE_COREJSON\
        NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT\
        PRINTF_ADVANCED_ENABLE=1\
        USE_RTOS=1"
)

mcux_add_iar_configuration(
    CC "--no_inline\
        --diag_suppress=Pa084,Pe188,Pe546"
)

mcux_add_mdk_configuration(
    CC "-Wno-typedef-redefinition -Wno-parentheses-equality"
    LD "--library_type=microlib"
)

# Add TF-M S import library
mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../el2go_agent_s/${CONFIG_TOOLCHAIN}/el2go_agent_s_CMSE_lib.o"
    TOOLCHAINS armgcc iar mdk
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
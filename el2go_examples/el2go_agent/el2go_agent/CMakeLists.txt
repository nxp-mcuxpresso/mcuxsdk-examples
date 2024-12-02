cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_agent LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/el2go_examples/el2go_agent)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/el2go_examples/el2go_agent/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/se_board_config.c
            examples/_boards/${board}/se_board_config.h
            examples/el2go_examples/common/enet/lwipopts.h
            examples/el2go_examples/common/sss/core_mqtt_config.h
            examples/el2go_examples/common/sss/core_pkcs11_config.h
            examples/el2go_examples/common/sss/FreeRTOSConfig.c
            examples/el2go_examples/common/sss/FreeRTOSConfig.h
            examples/el2go_examples/common/sss/FreeRTOSIPConfig.h
            examples/el2go_examples/common/sss/fsl_sss_ftr.h
            examples/el2go_examples/common/sss/sss_pkcs11.c
            middleware/nxp_iot_agent/ex/src/iot_agent_demo.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/el2go_examples/common/enet
             examples/el2go_examples/common/sss
)

mcux_add_macro(
    CC "MBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
        MBEDTLS_USER_CONFIG_FILE=\\\"sss_ksdk_mbedtls_config.h\\\"\
        NXP_IOT_AGENT_USE_COREJSON\
        NXP_IOT_AGENT_USE_MBEDTLS_TRANSPORT_FOR_MQTT\
        PRINTF_ADVANCED_ENABLE=1"
)

mcux_add_iar_configuration(
    CC "--diag_suppress=Pa084,Pa089,Pe188"
)

mcux_add_mdk_configuration(
    CC "-Wno-typedef-redefinition -Wno-parentheses-equality"
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
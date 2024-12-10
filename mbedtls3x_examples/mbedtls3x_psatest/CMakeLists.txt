cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mbedtls3x_psatest LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/mbedtls3x_examples/mbedtls3x_psatest)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
            mcux_mbedtls_psatest_config.h
)

mcux_add_include(
    INCLUDES /.
)

mcux_remove_macro("-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_psa_crypto_config.h\\\"")
mcux_add_macro(
    "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_psatest_config.h\\\""
)

include(${SdkRootDirPath}/examples/_boards/${board}/mbedtls3x_examples/mbedtls3x_psatest/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x4000\
             __heap_size__=0x8000"
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pe111,Pe188"
    CC "--diag_suppress Pe111,Pe188"
)
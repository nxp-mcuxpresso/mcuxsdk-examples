cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(psa_crypto_opaque_key_examples LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/mbedtls3x_examples/psa_crypto_opaque_key_examples)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/mbedtls3x_examples/psa_crypto_opaque_key_examples/psa_crypto_opaque_key_examples.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/mbedtls3x_examples/psa_crypto_opaque_key_examples
)

include(${SdkRootDirPath}/examples/_boards/${board}/mbedtls3x_examples/psa_crypto_opaque_key_examples/reconfig.cmake OPTIONAL)

mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_psa_crypto_opaque_config.h\\\""
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x4000\
             __heap_size__=0x1c000"
)

mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7"
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223"
    CC "--diag_suppress Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223"
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(psa_crypto_examples LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/mbedtls_examples/psa_crypto_examples)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/mbedtls_examples
    SOURCES psa_crypto_examples/crypto_examples.c
)

mcux_add_linker_symbol(
    SYMBOLS
        "__heap_size__=0x8000"
        "__stack_size__=0x4000"
)

include(${SdkRootDirPath}/${board_root}/${board}/mbedtls_examples/psa_crypto_examples/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

# IAR requires __write to be defined when dl7M_tln.a's putchar is pulled in via
# the tf-psa-crypto -> snprintf -> xprintffull_nomb -> _Prout -> putchar chain.
# SDK_DEBUGCONSOLE_UART enables the __write implementation in fsl_debug_console_lite.
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DSDK_DEBUGCONSOLE_UART"
    CX "-DSDK_DEBUGCONSOLE_UART"
)

#iar common configs
mcux_add_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050,Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223,Pa217"
    CX "--diag_suppress=Pa082,Pa050,Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223,Pa217"
)


cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(psa_crypto_opaque_key_examples LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/mbedtls_examples/psa_crypto_opaque_key_examples)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/mbedtls_examples
    SOURCES psa_crypto_opaque_key_examples/psa_crypto_opaque_key_examples.c
)

mcux_add_linker_symbol(
    SYMBOLS
        "__heap_size__=0xA000"
        "__stack_size__=0x4000"
)

include(${SdkRootDirPath}/${board_root}/${board}/mbedtls_examples/psa_crypto_opaque_key_examples/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

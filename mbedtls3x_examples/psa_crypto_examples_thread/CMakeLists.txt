cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(psa_crypto_examples_thread LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/mbedtls3x_examples/psa_crypto_examples_thread)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/mbedtls3x_examples/psa_crypto_examples_thread/psa_crypto_examples_thread.c
            examples/mbedtls3x_examples/psa_crypto_examples_thread/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/mbedtls3x_examples/psa_crypto_examples_thread
    examples/_boards/${board}
)

#add cc-define for all psa_crypto_examples_thread 
mcux_add_macro(
     CC "-DMBEDTLS_PLATFORM_MEMORY\
        -DMBEDTLS_THREADING_ALT\
        -DMBEDTLS_THREADING_C\
        -DPSA_CRYPTO_DRIVER_THREAD_EN\
        -DFREESCALE_KSDK_BM"
)

#armgcc common configs
mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable"
)

#iar common configs
mcux_add_iar_configuration(
    CC "--diag_suppress Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223"
    CX "--diag_suppress Pe546,Pe188,Pe111,Pa084,Pe068,Pe177,Pe223"
)

#mdk common configs
mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7"
)

#enable heap/stack
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x8000"
)


include(${SdkRootDirPath}/examples/_boards/${board}/mbedtls3x_examples/psa_crypto_examples_thread/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
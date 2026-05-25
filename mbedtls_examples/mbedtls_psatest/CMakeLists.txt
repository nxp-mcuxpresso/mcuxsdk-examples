cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mbedtls_psatest LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/mbedtls_examples/mbedtls_psatest)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
)

mcux_add_include(
    INCLUDES /.
)

include(${SdkRootDirPath}/${board_root}/${board}/mbedtls_examples/mbedtls_psatest/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x4000\
             __heap_size__=0x8000"
)

mcux_add_mdk_configuration(
    CC "-Wno-typedef-redefinition"
    CX "-Wno-typedef-redefinition"
)
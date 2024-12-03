cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mbedtls3x_benchmark LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/mbedtls3x_examples/mbedtls3x_benchmark)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES benchmark.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/mbedtls3x_examples/mbedtls3x_benchmark/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_iar_configuration(
    CX "--diag_suppress Pe111"
    CC "--diag_suppress Pe111"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x8000"
)
	
mcux_add_macro(
    CC  "-DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1"
)
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mbedtls_benchmark LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/mbedtls_examples/mbedtls_benchmark)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/mbedtls_examples/mbedtls_benchmark/benchmark.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/mbedtls_examples/mbedtls_benchmark/
)

include(${SdkRootDirPath}/${board_root}/${board}/mbedtls_examples/mbedtls_benchmark/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x8000"
)

# Remove default release target optimization
mcux_remove_armgcc_configuration(
    CC "-Os"
)

mcux_remove_iar_configuration(
    CC "-Oh"
)

mcux_remove_mdk_configuration(
    CC "-Oz"
)

# Remove default debug target optimization
mcux_remove_armgcc_configuration(
    CC "-O0"
)

mcux_remove_iar_configuration(
    CC "-On"
)

mcux_remove_mdk_configuration(
    CC "-O1"
)


# Add fast optimization
mcux_add_armgcc_configuration(
    CC "-Ofast"
)

mcux_add_mdk_configuration(
    CC "-Ofast"
)

mcux_add_iar_configuration(
    CC "-Ohs"
)
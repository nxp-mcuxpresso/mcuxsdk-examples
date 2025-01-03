cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(els_hash LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/els_pkc_examples/els_hash)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/els_pkc_examples/els_hash/els_hash.c
    examples/els_pkc_examples/els_hash/els_hash.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES /.
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x4000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/els_pkc_examples/els_hash/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
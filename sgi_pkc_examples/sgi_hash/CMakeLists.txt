cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sgi_hash LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/sgi_pkc_examples/sgi_hash)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/sgi_pkc_examples/sgi_hash/sgi_hash.c
            examples/sgi_pkc_examples/sgi_hash/sgi_hash.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/sgi_pkc_examples/sgi_hash
             ${board_root}/${board}/sgi_pkc_examples/sgi_hash/${core_id}
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x4000"
)
include(${SdkRootDirPath}/${board_root}/${board}/sgi_pkc_examples/sgi_hash/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
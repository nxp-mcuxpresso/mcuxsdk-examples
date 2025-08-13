cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sgi_symmetric LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/sgi_pkc_examples/sgi_symmetric)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/sgi_pkc_examples/sgi_symmetric/sgi_symmetric.c
            examples/sgi_pkc_examples/sgi_symmetric/sgi_symmetric.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/sgi_pkc_examples/sgi_symmetric
             ${board_root}/${board}/sgi_pkc_examples/sgi_symmetric/${core_id}
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x4000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/sgi_pkc_examples/sgi_symmetric/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
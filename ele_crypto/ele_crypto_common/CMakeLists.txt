cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_crypto_common LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ele_crypto/ele_crypto_common)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_crypto_common.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000\
             __heap_size__=0x8000"
)

include(${SdkRootDirPath}/${board_root}/${board}/ele_crypto/ele_crypto_common/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

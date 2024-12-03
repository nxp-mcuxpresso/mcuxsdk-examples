cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_crypto_nvm_generic LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ele_crypto/ele_crypto_nvm_generic)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_crypto_nvm_generic.c
)

mcux_add_include(
    INCLUDES .
)

# Add source for the loadable ELE FW
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ele_crypto/ele_crypto_nvm_generic/ele_fw.h
)

# Modify stack and heap sizes for all IDEs
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800\
             __heap_size__=0x600"
)

include(${SdkRootDirPath}/examples/_boards/${board}/ele_crypto/ele_crypto_nvm_generic/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

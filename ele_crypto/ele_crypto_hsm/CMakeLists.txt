cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_crypto_hsm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ele_crypto/ele_crypto_hsm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_crypto_hsm.c
)

mcux_add_include(
    INCLUDES .
)

# Add source for the loadable ELE FW
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ele_crypto/ele_crypto_hsm/ele_fw.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/ele_crypto/ele_crypto_hsm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

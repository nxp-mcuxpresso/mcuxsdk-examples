cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(els_symmetric LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/els_pkc_examples/els_symmetric)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/els_pkc_examples/els_symmetric/els_symmetric.c
    examples/els_pkc_examples/els_symmetric/els_symmetric.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES /.
)

include(${SdkRootDirPath}/examples/_boards/${board}/els_pkc_examples/els_symmetric/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
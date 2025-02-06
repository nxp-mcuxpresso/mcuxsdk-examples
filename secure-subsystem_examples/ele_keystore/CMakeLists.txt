cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_keystore LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/secure-subsystem_examples/ele_keystore)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_keystore.c
)

mcux_add_include(
    INCLUDES .
)

include(${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_keystore/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

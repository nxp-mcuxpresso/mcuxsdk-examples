cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_decrypt_sb3 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/secure-subsystem_examples/ele_decrypt_sb3)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_decrypt_sb3.c
)

mcux_add_include(
    INCLUDES .
)

include(${SdkRootDirPath}/examples/_boards/${board}/secure-subsystem_examples/ele_decrypt_sb3/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

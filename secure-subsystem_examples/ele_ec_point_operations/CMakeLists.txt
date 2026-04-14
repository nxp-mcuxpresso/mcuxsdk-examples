cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_ec_point_operations LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/secure-subsystem_examples/ele_ec_point_operations)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_ec_point_operations.c
)

mcux_add_include(
    INCLUDES .
)

include(${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_ec_point_operations/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(openvg_tiger_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/openvg_examples/openvg_tiger)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES tiger_freertos.c
            tiger_path.h
)

mcux_add_include(
    INCLUDES .
)

include(${SdkRootDirPath}/examples/_boards/${board}/openvg_examples/openvg_tiger/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

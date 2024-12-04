cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(project_template LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/project_template)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/project_template/main.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/project_template/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
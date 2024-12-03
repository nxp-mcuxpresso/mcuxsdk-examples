cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fxls8974cf_interrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/shields/a8974/issdk_examples/sensors/fxls8974cf/fxls8974cf_interrupt)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES fxls8974cf_interrupt.c
)

mcux_add_include(
    INCLUDES .
)

include(${SdkRootDirPath}/examples/_boards/${board}/shields/a8974/issdk_examples/sensors/fxls8974cf/fxls8974cf_interrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

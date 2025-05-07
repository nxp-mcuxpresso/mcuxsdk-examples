cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(magnetic_switch LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/magnetic_switch)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/magnetic_switch/main.c
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/magnetic_switch/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
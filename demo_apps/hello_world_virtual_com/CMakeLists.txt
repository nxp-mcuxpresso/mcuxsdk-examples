cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hello_world_virtual_com LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/hello_world_virtual_com)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/hello_world/hello_world.c
)

mcux_add_iar_configuration(
  LD "--semihosting"
)
mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/hello_world
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/hello_world_virtual_com/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
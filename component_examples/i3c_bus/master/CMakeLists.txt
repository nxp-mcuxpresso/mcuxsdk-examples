cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(i3c_bus_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/i3c_bus/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES components/i3c_bus/example/master/i3c_bus_master.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/component_examples/i3c_bus/master
             examples/_boards/${board}/component_examples/i3c_bus/master/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/i3c_bus/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
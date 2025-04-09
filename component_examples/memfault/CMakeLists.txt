cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(memfault LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/memfault)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES main.c
            FreeRTOSConfig_frag.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples
    INCLUDES _boards/${board}/component_examples/memfault
             component_examples/memfault
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/memfault/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

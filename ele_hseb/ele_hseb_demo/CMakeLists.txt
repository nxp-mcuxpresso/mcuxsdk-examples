cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_hseb_demo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/ele_hseb/ele_hseb_demo)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ele_hseb_demo.c
)

mcux_add_include(
    INCLUDES .
)

# Modify stack and heap sizes for all IDEs
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800\
             __heap_size__=0x600"
)

# Pragma push and pop operations are defined in header file,
# MDK doesn't like this and generate warnings.
# This is already disabled at the component level, but we make sure
# to define it here as well for correct CMSIS pack generation.
mcux_add_mdk_configuration(
    CC "-Wno-pragma-pack"
    CX "-Wno-pragma-pack"
)

include(${SdkRootDirPath}/${board_root}/${board}/ele_hseb/ele_hseb_demo/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

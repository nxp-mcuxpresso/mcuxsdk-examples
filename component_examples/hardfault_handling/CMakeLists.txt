cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hardfault_handling LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/hardfault_handling)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES components/exception_handling/example/hardfault_handling.c
)

mcux_add_iar_configuration(
  LD "--semihosting"
)
mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/hardfault_handling/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(semihost LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/semihost)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/semihost/semihost.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/component_examples/semihost
             ${board_root}/${board}/component_examples/semihost/${core_id}
)

# IAR routes stdio through semihosting when the linker semihosting library is used.
mcux_add_iar_configuration(
  LD "--semihosting"
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/semihost/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

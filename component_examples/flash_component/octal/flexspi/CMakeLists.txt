cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flash_component_octal_flexspi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/flash_component/flexspi_octal)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/flash_component/octal/octal_flash.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/component_examples/flash_component/flexspi_octal
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/flash_component/flexspi_octal/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

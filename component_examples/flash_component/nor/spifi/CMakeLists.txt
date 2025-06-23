cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flash_component_nor_spifi LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/flash_component/spifi_nor)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/flash_component/nor/nor_flash.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/component_examples/flash_component/spifi_nor/${core_id}
             ${board_root}/${board}/component_examples/flash_component/spifi_nor
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/flash_component/spifi_nor/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

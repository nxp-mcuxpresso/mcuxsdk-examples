cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(virt_wrapper_siul2 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/virt_wrapper/siul2)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES virt_wrapper_siul2.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/virt_wrapper/siul2/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/virt_wrapper/siul2/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
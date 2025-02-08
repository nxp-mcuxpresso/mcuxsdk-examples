cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mcxw23_romapi_flash LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/romapi/flashiap)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/mcxw_romapi/flashiap/flashiap.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/romapi/flashiap
             ${board_root}/${board}/driver_examples/romapi/flashiap/${core_id}
)

mcux_add_macro(
    CC "-DENABLE_RAM_VECTOR_TABLE"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/romapi/flashiap/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
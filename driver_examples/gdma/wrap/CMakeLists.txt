cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(gdma_wrap LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/gdma/wrap)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/gdma/wrap/gdma_wrap.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/gdma/wrap
             ${board_root}/${board}/driver_examples/gdma/mem2mem
             ${board_root}/${board}/driver_examples/gdma/wrap/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/gdma/wrap/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
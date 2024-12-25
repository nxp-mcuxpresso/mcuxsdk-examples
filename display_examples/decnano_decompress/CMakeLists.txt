
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(decnano_decompress LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/display_examples/decnano_decompress)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/decnano_decompress/decnano_decompress.c
    		examples/display_examples/decnano_decompress/input.c
    		examples/display_examples/decnano_decompress/input.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
             ${board_root}/${board}/display_examples/decnano_decompress
             ${board_root}/${board}/display_examples/decnano_decompress/${multicore_foldername}
             examples/display_examples/decnano_decompress
)

include(${SdkRootDirPath}/${board_root}/${board}/display_examples/decnano_decompress/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

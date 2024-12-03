
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pngdec LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/pngdec)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/pngdec/pngdec.c
            examples/driver_examples/pngdec/png.c
            examples/driver_examples/pngdec/png.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/_boards/${board}/driver_examples/pngdec
             examples/_boards/${board}/driver_examples/pngdec/${multicore_foldername}
             examples/driver_examples/pngdec/
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/pngdec/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
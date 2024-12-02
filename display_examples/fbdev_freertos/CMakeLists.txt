
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fbdev_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/display_examples/fbdev_freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/fbdev_freertos/fbdev_freertos.c
            examples/_boards/${board}/display_support.h
            examples/_boards/${board}/display_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/_boards/${board}/display_examples/fbdev_freertos
             examples/_boards/${board}/display_examples/fbdev_freertos/${multicore_foldername}
             examples/display_examples/fbdev_freertos
)

include(${SdkRootDirPath}/examples/_boards/${board}/display_examples/fbdev_freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

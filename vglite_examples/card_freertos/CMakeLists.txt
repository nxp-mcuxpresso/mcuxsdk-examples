cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(card_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/vglite_examples/card_freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/card_freertos/card_freertos.c
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.c
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/vglite/vglite_sdk/example/card_freertos
             middleware/vglite/vglite_sdk/example/vglite_window
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pe257,Pa039"
    CC "--diag_suppress=Pe257,Pa039,Pe188,Pa089,Pe191,Pe111,Pa093,Pe177,Pe546,Pe236,Pa205,Pe136,Pa084,Pe550,Pe186"
    CX "--diag_suppress=Pe257,Pa039,Pe188,Pa089,Pe191,Pe111,Pa093,Pe177,Pe546,Pe236,Pa205,Pe136,Pa084,Pe550,Pe186"
)

include(${SdkRootDirPath}/${board_root}/${board}/vglite_examples/card_freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

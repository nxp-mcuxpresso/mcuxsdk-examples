cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cube_raster_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/vglite_examples/cube_raster_freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/vglite/vglite_sdk/example/cube_raster_freertos/cube_freertos.c
    SOURCES middleware/vglite/vglite_sdk/example/cube_raster_freertos/draw_cube.c
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/draw_cube.h
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/tiger_blue.h
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/tiger_laven.h
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/tiger_lime.h
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/tiger_turk.h
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/tiger_white.h
            middleware/vglite/vglite_sdk/example/cube_raster_freertos/tiger_yellow.h
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.c
            middleware/vglite/vglite_sdk/example/vglite_window/vglite_window.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/vglite/vglite_sdk/example/cube_raster_freertos
             middleware/vglite/vglite_sdk/example/vglite_window
)

include(${SdkRootDirPath}/${board_root}/${board}/vglite_examples/cube_raster_freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

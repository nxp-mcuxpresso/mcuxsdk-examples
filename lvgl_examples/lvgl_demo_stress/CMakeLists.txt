cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lvgl_demo_stress LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lvgl_examples/lvgl_demo_stress)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    INCLUDES ..
)

mcux_add_source(
    SOURCES lvgl_demo_stress.c
            ../lvgl_demo_utils.c
            ../lvgl_demo_utils.h
	    ../lvgl_freertos.c
            ../lvgl_freertos.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/lvgl_examples/lvgl_demo_stress/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

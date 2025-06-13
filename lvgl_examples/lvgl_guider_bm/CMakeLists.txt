cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lvgl_guider_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/lvgl_examples/lvgl_guider_bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    INCLUDES ./..
             ./generated
             ./custom
             ./generated/guider_customer_fonts
)

mcux_add_source(
    SOURCES ./lvgl_guider_bm.c
            ./generated/guider_customer_fonts/*.h
            ./generated/images/*.h
            ./generated/images/*.c
            ./generated/*.c
            ./generated/*.h
            ./custom/*.c
            ./custom/*.h
            ./../lvgl_demo_utils.c
            ./../lvgl_demo_utils.h
)

include(${SdkRootDirPath}/${board_root}/${board}/lvgl_examples/lvgl_guider_bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

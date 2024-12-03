cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lvgl_guider LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lvgl_examples/lvgl_guider)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    INCLUDES ./..
             ./generated
             ./custom
             ./generated/guider_customer_fonts
)

mcux_add_source(
    SOURCES ./lvgl_guider.c
            ./generated/guider_customer_fonts/guider_customer_fonts.h
            ./generated/gui_guider.c
            ./generated/gui_guider.h
            ./generated/events_init.c
            ./generated/events_init.h
            ./generated/setup_scr_screen.c
            ./generated/widgets_init.c
            ./generated/widgets_init.h
            ./custom/custom.c
            ./custom/custom.h
            ./custom/lv_conf_ext.h
            ./../lvgl_demo_utils.c
            ./../lvgl_demo_utils.h
            ./../lvgl_freertos.c
            ./../lvgl_freertos.h
)

include(${SdkRootDirPath}/examples/_boards/${board}/lvgl_examples/lvgl_guider/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

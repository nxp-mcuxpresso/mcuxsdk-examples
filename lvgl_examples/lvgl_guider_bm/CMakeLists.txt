cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lvgl_guider_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/lvgl_examples/lvgl_guider_bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    INCLUDES ./..
             ./generated
             ./custom
             ./generated/assets/fonts
             ./generated/assets/images
             ./generated/events
             ./generated/screens
)

mcux_add_source(
    SOURCES ./lvgl_guider_bm.c
            ./generated/assets/fonts/gg_font.h
            ./generated/assets/images/gg_image.h
            ./generated/events/gg_event_layer_bottom.c
            ./generated/events/gg_event_layer_sys.c
            ./generated/events/gg_event_layer_top.c
            ./generated/events/gg_event_screen.c
            ./generated/events/gg_event.h
            ./generated/screens/gg_layer_bottom.c
            ./generated/screens/gg_layer_sys.c
            ./generated/screens/gg_layer_top.c
            ./generated/screens/gg_screen.c
            ./generated/gg_utils.c
            ./generated/gg_utils.h
            ./generated/gui_guider.h
            ./custom/custom.c
            ./custom/custom.h
            ./custom/lv_conf_ext.h
            ./../lvgl_demo_utils.c
            ./../lvgl_demo_utils.h
)

include(${SdkRootDirPath}/${board_root}/${board}/lvgl_examples/lvgl_guider_bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

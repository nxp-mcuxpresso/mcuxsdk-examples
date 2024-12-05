
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/pin_mux.c
            ${board_root}/${board}/lvgl_examples/pin_mux.h
            ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_demo_stress
             ${board_root}/${board}/lvgl_examples
)


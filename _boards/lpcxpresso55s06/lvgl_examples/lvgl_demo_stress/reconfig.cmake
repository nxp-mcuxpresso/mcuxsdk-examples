
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/pin_mux.c
            ${board_root}/${board}/lvgl_examples/pin_mux.h
            ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)


mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

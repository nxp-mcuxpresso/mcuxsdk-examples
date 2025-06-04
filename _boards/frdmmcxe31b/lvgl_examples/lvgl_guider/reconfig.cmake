
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples
)

mcux_add_macro(
    CC "-DBOARD_LCD_S035=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)

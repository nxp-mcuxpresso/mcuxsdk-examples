
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_demo_benchmark_bm
             ${board_root}/${board}/lvgl_examples
)

mcux_add_macro(
    CC "-DBOARD_LCD_S035=1"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1800"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1800\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1800"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/lvgl_examples/pin_mux.c
            examples/_boards/${board}/lvgl_examples/pin_mux.h
            examples/_boards/${board}/lvgl_examples/hardware_init.c
            examples/_boards/${board}/lvgl_examples/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/lvgl_examples
             examples/_boards/${board}/lvgl_examples/lvgl_demo_benchmark_bm
)


mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000"
)

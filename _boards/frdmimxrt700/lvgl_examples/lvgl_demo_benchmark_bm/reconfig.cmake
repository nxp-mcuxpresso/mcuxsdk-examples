
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

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000"
)

include(${SdkRootDirPath}/${board_root}/${board}/lvgl_examples/reconfig.cmake)

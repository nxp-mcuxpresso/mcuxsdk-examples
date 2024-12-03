
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
)

include(${SdkRootDirPath}/examples/_boards/${board}/lvgl_examples/reconfig.cmake)

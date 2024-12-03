
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/project_template/peripherals.c
            examples/_boards/${board}/project_template/peripherals.h
            examples/_boards/${board}/project_template/pin_mux.c
            examples/_boards/${board}/project_template/pin_mux.h
            examples/_boards/${board}/demo_apps/led_blinky/hardware_init.c
            examples/_boards/${board}/demo_apps/led_blinky/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/project_template
             examples/_boards/${board}/demo_apps/led_blinky
)

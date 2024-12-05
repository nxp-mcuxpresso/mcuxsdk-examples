
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/project_template/peripherals.c
            ${board_root}/${board}/project_template/peripherals.h
            ${board_root}/${board}/project_template/pin_mux.c
            ${board_root}/${board}/project_template/pin_mux.h
            ${board_root}/${board}/demo_apps/led_blinky/hardware_init.c
            ${board_root}/${board}/demo_apps/led_blinky/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/project_template
             ${board_root}/${board}/demo_apps/led_blinky
)

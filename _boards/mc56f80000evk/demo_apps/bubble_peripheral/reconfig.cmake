
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/bubble_peripheral/bubble.c
            ${board_root}/${board}/demo_apps/bubble_peripheral/peripherals.c
            ${board_root}/${board}/demo_apps/bubble_peripheral/peripherals.h
)

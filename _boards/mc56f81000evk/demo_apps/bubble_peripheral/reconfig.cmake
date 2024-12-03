
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/bubble_peripheral/bubble.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/bubble_peripheral/peripherals.h
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/utick_wakeup_xip_peripheral/peripherals.c
            examples/_boards/${board}/demo_apps/utick_wakeup_xip_peripheral/peripherals.h
)

mcux_add_iar_configuration(
    LD "--config_def XIP_IMAGE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    AS "-DXIP_IMAGE"
)

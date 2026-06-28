
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/mcu_bootloader/boards/${board}/flashloader/pin_mux.c
            middleware/mcu_bootloader/boards/${board}/flashloader/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/mcu_bootloader/boards/${board}/flashloader
)

mcux_add_macro(
    TOOLCHAINS mdk
    AS "-DBL_HAS_BOOTLOADER_CONFIG=0"
)
mcux_add_macro(
    CC "-DBL_TARGET_RAM\
       -DMIMXRT1152\
       -DUSB_STACK_BM"
)

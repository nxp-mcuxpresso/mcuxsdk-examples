
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/romapi/bootloader/romapi_bootloader.c
            ${board_root}/${board}/driver_examples/romapi/common/pin_mux.c
            ${board_root}/${board}/driver_examples/romapi/common/pin_mux.h
            ${board_root}/${board}/driver_examples/romapi/common/hardware_init.c
            ${board_root}/${board}/driver_examples/romapi/common/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/romapi/common
)
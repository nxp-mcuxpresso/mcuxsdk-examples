

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/common/hardware_init.c
            ${board_root}/${board}/wifi_examples/common/app.h
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
)


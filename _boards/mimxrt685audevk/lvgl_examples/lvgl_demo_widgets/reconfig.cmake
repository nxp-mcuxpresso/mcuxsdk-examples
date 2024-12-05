
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/lvgl_examples/pin_mux.c
            ${board_root}/${board}/lvgl_examples/pin_mux.h
            ${board_root}/${board}/lvgl_examples/hardware_init.c
            ${board_root}/${board}/lvgl_examples/app.h
            ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/lvgl_examples
)


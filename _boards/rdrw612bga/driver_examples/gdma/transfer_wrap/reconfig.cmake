
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/gdma/mem2mem/pin_mux.c
            ${board_root}/${board}/driver_examples/gdma/mem2mem/pin_mux.h
            ${board_root}/${board}/driver_examples/gdma/mem2mem/hardware_init.c
            ${board_root}/${board}/driver_examples/gdma/mem2mem/app.h
)

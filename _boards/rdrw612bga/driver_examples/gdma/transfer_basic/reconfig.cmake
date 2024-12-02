
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/gdma/mem2mem/pin_mux.c
            examples/_boards/${board}/driver_examples/gdma/mem2mem/pin_mux.h
            examples/_boards/${board}/driver_examples/gdma/mem2mem/hardware_init.c
            examples/_boards/${board}/driver_examples/gdma/mem2mem/app.h
)

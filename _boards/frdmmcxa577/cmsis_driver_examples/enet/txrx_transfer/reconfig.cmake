mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/enet/txrx_transfer/hardware_init.c
            ${board_root}/${board}/cmsis_driver_examples/enet/txrx_transfer/app.h
)

mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x3000\
             __stack_size__=0x3000\
             __ram_vector_table__=1"
)

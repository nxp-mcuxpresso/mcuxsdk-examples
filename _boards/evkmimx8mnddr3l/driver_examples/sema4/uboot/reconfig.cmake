
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/sema4/uboot/./pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/sema4/uboot/./pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/sema4/uboot/./hardware_init.c
            examples/_boards/evkmimx8mn/driver_examples/sema4/uboot/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/sema4/uboot/.
             examples/_boards/evkmimx8mn
)

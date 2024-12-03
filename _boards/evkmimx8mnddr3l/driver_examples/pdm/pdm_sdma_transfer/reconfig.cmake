
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sdma_transfer/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sdma_transfer/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sdma_transfer/./app.h
            examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sdma_transfer/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sdma_transfer
             examples/_boards/evkmimx8mn
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

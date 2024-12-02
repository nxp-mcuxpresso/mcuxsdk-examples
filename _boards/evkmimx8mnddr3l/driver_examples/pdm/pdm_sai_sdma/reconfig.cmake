
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sai_sdma/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sai_sdma/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sai_sdma/./app.h
            examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sai_sdma/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/pdm/pdm_sai_sdma
             examples/_boards/evkmimx8mn
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/./app.h
            examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_sdma
             examples/driver_examples/easrc/asrc_m2m_sdma
             examples/_boards/evkmimx8mn
)

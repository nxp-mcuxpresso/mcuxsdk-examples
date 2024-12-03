
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/./app.h
            examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/asrc/asrc_m2m_polling
             examples/driver_examples/easrc/asrc_m2m_polling
             examples/_boards/evkmimx8mn
)

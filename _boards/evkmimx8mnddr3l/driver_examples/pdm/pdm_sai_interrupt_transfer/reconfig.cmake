
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_sai_interrupt_transfer/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_sai_interrupt_transfer/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_sai_interrupt_transfer/./app.h
            ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_sai_interrupt_transfer/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_sai_interrupt_transfer
             ${board_root}/evkmimx8mn
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_interrupt/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_interrupt/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_interrupt/./app.h
            ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_interrupt/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/pdm/pdm_interrupt
             ${board_root}/evkmimx8mn
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

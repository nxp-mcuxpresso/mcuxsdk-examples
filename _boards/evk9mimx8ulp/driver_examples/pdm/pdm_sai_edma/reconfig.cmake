
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/pdm/pdm_sai_edma/./app.h
            ${board_root}/evkmimx8ulp/driver_examples/pdm/pdm_sai_edma/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/pdm/pdm_sai_edma
             ${board_root}/evkmimx8ulp
)

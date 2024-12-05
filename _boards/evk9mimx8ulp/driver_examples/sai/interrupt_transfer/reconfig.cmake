
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/sai/interrupt_transfer/./app.h
            ${board_root}/evkmimx8ulp/driver_examples/sai/interrupt_transfer/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/sai/interrupt_transfer
             ${board_root}/evkmimx8ulp
             examples/driver_examples/sai
)

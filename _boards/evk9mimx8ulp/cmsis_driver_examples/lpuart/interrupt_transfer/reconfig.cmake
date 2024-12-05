mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/lpuart/interrupt_transfer/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/cmsis_driver_examples/lpuart/interrupt_transfer/hardware_init.c
            ${board_root}/evkmimx8ulp/cmsis_driver_examples/lpuart/interrupt_transfer/app.h
            ${board_root}/evkmimx8ulp/cmsis_driver_examples/lpuart/interrupt_transfer/RTE_Device.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/cmsis_driver_examples/lpuart/interrupt_transfer
             ${board_root}/evkmimx8ulp
)

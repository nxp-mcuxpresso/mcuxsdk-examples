mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/slave/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/pin_mux.c
            ${board_root}/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/pin_mux.h
            ${board_root}/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/hardware_init.c
            ${board_root}/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/app.h
            ${board_root}/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/RTE_Device.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave
             ${board_root}/evkmimx8mn
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

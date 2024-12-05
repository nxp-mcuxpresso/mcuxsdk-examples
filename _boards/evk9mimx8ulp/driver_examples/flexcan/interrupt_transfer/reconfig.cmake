
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/flexcan/interrupt_transfer/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/flexcan/interrupt_transfer/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/flexcan/interrupt_transfer/.
             ${board_root}/evkmimx8ulp
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
)

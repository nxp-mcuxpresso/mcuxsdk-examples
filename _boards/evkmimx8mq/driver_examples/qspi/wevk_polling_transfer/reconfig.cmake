
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/qspi/polling_transfer/./app.h
            ${board_root}/${board}/driver_examples/qspi/polling_transfer/hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/qspi/polling_transfer/.
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

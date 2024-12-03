mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/${core_id}/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/slave/${core_id}
)

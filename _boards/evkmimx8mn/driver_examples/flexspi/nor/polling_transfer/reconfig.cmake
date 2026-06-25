include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/flexspi/nor/polling_transfer/pin_mux.c
            ${board_root}/${board}/driver_examples/flexspi/nor/polling_transfer/hardware_init.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/flexspi/nor/polling_transfer
)

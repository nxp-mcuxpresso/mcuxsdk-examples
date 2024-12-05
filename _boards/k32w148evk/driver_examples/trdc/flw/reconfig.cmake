
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/driver_examples/trdc/flw/./hardware_init.c
            ${board_root}/kw45b41zevk/driver_examples/trdc/flw/./app.h
            ${board_root}/${board}/driver_examples/trdc/basic/pin_mux.c
            ${board_root}/${board}/driver_examples/trdc/basic/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/driver_examples/trdc/flw/.
             ${board_root}/${board}/driver_examples/trdc/basic
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/elcdif/elcdif_support.h
            ${board_root}/${board}/driver_examples/elcdif/elcdif_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/elcdif
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

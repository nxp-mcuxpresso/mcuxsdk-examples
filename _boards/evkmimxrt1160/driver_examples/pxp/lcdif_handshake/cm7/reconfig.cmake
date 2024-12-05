
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/pxp/elcdif_support.h
            ${board_root}/${board}/driver_examples/pxp/elcdif_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/pxp
)

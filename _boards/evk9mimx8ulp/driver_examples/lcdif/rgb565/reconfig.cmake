mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/lcdif/lcdif_support.h
            ${board_root}/${board}/driver_examples/lcdif/lcdif_support.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/lcdif/lcdif_support.h
            ${board_root}/evkmimx8ulp/driver_examples/lcdif/lcdif_support.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/lcdif
             ${board_root}/evkmimx8ulp
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
)

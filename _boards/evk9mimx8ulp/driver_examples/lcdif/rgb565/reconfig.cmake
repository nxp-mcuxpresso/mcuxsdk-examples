mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/lcdif/lcdif_support.h
            examples/_boards/${board}/driver_examples/lcdif/lcdif_support.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/lcdif/lcdif_support.h
            examples/_boards/evkmimx8ulp/driver_examples/lcdif/lcdif_support.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/lcdif
             examples/_boards/evkmimx8ulp
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
)

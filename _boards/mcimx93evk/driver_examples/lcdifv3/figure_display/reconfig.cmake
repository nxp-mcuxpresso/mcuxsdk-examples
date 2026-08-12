
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/lcdifv3/${multicore_foldername}/pin_mux.c
            ${board_root}/${board}/driver_examples/lcdifv3/${multicore_foldername}/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lcdifv3/${multicore_foldername}
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_ADP5585=1\
       -DRM67199_ENABLE=1"
)

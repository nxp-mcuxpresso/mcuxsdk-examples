
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1"
)

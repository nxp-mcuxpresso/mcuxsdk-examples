
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_mdk_configuration(
    CC "-Wno-implicit\
       -Os"
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

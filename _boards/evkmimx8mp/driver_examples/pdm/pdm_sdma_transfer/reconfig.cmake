
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

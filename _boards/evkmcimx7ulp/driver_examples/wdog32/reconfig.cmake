
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS flash_debug flash_release
    CC "-DENABLE_RAM_VECTOR_TABLE"
)

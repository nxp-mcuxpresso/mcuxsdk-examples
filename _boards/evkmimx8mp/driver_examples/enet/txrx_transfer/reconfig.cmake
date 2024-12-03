
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_macro(
    CC "-DPHY_STABILITY_DELAY_US=500000U\
       -DPHY_AUTONEGO_TIMEOUT_COUNT=300000U"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS ddr_debug ddr_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

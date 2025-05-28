
mcux_add_macro(
    CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
)

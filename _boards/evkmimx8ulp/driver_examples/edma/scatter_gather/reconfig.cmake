
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_armgcc_configuration(
    AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
)
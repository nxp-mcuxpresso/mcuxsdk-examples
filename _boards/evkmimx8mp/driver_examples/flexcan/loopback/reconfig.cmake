
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

#flexcan module functionality is different between CPU_MIMX8ML8CVNKZ and CPU_MIMX8ML8DVNLZ, the board definitions need to be added to support this example
mcux_add_configuration(
        CC "-DCPU_MIMX8ML8DVNLZ=1"
)
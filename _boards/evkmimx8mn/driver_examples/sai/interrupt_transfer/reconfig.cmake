
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

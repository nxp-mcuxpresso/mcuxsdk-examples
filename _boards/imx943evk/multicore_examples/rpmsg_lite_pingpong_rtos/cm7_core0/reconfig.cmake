# Add additional configuration
mcux_add_macro(
    CC "-DRPMSG_M70_MASTER=1"
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos/${core_id}/rpmsg_config.h
)

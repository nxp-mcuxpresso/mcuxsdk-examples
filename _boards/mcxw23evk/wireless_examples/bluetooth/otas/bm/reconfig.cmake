include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/bluetooth/otas/${multicore_foldername}/reconfig.cmake OPTIONAL)

# the iar debug target requires a bit more stack
mcux_add_configuration(
    TOOLCHAINS iar
    TARGETS debug
    LD "--config_def=__stack_size__=0xA00"
)

include(${SdkRootDirPath}/examples_int/_boards/${board}/wireless_examples/bluetooth/loc_reader/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x0CE4"
)

mcux_add_armgcc_configuration(
    CC "-Xlinker --defsym=__stack_size__=0x0CE4"
)

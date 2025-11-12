include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/bluetooth/digital_key_car_anchor_cs/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x0BB8"
)

mcux_add_armgcc_configuration(
    CC "-Xlinker --defsym=__stack_size__=0x0BB8"
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/i3c_bus/master/i3c_bus_master.c
            examples/_boards/evkmimx8ulp/component_examples/i3c_bus/master/hardware_init.c
            examples/_boards/evkmimx8ulp/component_examples/i3c_bus/master/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/component_examples/i3c_bus/master
             examples/_boards/evkmimx8ulp
)

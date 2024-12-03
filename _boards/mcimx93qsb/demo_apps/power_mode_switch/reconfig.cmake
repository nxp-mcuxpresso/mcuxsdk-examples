
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/demo_apps/power_mode_switch/rpmsg_config.h
            examples/demo_apps/power_mode_switch_imx93/power_mode_switch.h
            examples/demo_apps/power_mode_switch_imx93/lpm.c
            examples/demo_apps/power_mode_switch_imx93/lpm.h
            examples/_boards/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/demo_apps/power_mode_switch_imx93
)

mcux_add_macro(
    AS "-DNO_CRP"
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)

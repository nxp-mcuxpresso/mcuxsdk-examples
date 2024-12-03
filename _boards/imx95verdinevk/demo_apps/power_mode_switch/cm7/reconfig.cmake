mcux_add_configuration(
	CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_ADP5585=1\
        -DSCMI_LM_REQUEST_M7_SUSPEND_ENABLE=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_switch/cm7/rpmsg_config.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/srtm_config.h
            examples/demo_apps/power_mode_switch_imx95/power_mode_switch.h
            examples/demo_apps/power_mode_switch_imx95/lpm.c
            examples/demo_apps/power_mode_switch_imx95/lpm.h
            examples/_boards/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/_boards/${board}/cm7/app_srtm.c
            examples/_boards/${board}/cm7/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/demo_apps/power_mode_switch_imx95
             examples/_boards/${board}/cm7
             examples/_boards/${board}/demo_apps/power_mode_switch
)

mcux_add_macro(
    AS "-DNO_CRP"
)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)


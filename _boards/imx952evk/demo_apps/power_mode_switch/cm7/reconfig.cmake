mcux_add_configuration(
	CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_ADP5585=1\
        -DSCMI_LM_REQUEST_M7_SUSPEND_ENABLE=1\
        -DSCMI_LMM_POWER_CHANGE_PROCESSED=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/power_mode_switch/cm7/rpmsg_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/srtm_config.h
            examples/demo_apps/power_mode_switch_imx952/power_mode_switch.h
            examples/demo_apps/power_mode_switch_imx952/lpm.c
            examples/demo_apps/power_mode_switch_imx952/lpm.h
            ${board_root}/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/cm7/app_srtm.c
            ${board_root}/${board}/cm7/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/demo_apps/power_mode_switch_imx952
             ${board_root}/${board}/cm7
             ${board_root}/${board}/demo_apps/power_mode_switch
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



mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/demo_apps/power_mode_switch/rpmsg_config.h
            examples/demo_apps/power_mode_switch_imx93/power_mode_switch.h
            examples/demo_apps/power_mode_switch_imx93/lpm.c
            examples/demo_apps/power_mode_switch_imx93/lpm.h
            ${board_root}/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/demo_apps/power_mode_switch_imx93
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_ADP5585=1\
       -DBOARD_USE_DDR_RETENTION=1"
    AS "-DNO_CRP"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)

mcux_add_configuration(
           CC "-DBOARD_USE_PCA6416A=1"
)

mcux_add_configuration(
           CC "-DBOARD_USE_TPM=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/demo_apps/power_mode_switch//./hardware_init.c
            ${board_root}/evkmimx8ulp/demo_apps/power_mode_switch//./app.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/rsc_table.c
            ${board_root}/evkmimx8ulp/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/evkmimx8ulp/demo_apps/power_mode_switch/rpmsg_config.h
            ${board_root}/evkmimx8ulp/srtm_config.h
            ${board_root}/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.c
            ${board_root}/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.h
            examples/demo_apps/power_mode_switch_imx8ulp/power_mode_switch.h
            examples/demo_apps/power_mode_switch_imx8ulp/lpm.c
            examples/demo_apps/power_mode_switch_imx8ulp/lpm.h
            ${board_root}/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            ${board_root}/evkmimx8ulp/FreeRTOSConfigBoard.h
            ${board_root}/evkmimx8ulp/demo_apps/power_mode_switch/./hardware_init.c
            ${board_root}/evkmimx8ulp/demo_apps/power_mode_switch/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            ${board_root}/evkmimx8ulp/app_srtm.c
            ${board_root}/evkmimx8ulp/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp
             middleware/multicore/remoteproc
             ${board_root}/evkmimx8ulp/demo_apps/power_mode_switch
             ${board_root}/evkmimx8ulp/drivers/lpi2c
             examples/demo_apps/power_mode_switch_imx8ulp
)

mcux_add_macro(
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

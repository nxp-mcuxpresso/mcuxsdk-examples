mcux_add_configuration(
	   CC "-DBOARD_USE_PCA6416A=1"
)
mcux_add_configuration(
	   CC "-DBOARD_USE_TPM=1"
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_switch/rpmsg_config.h
    	    examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/drivers/lpi2c/fsl_lpi2c_freertos.c
            examples/_boards/${board}/drivers/lpi2c/fsl_lpi2c_freertos.h
            examples/demo_apps/power_mode_switch_imx8ulp/power_mode_switch.h
            examples/demo_apps/power_mode_switch_imx8ulp/lpm.c
            examples/demo_apps/power_mode_switch_imx8ulp/lpm.h
            examples/_boards/${board}/demo_apps/power_mode_switch/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/_boards/${board}/drivers/lpi2c
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

mcux_remove_iar_configuration(
  TARGETS flash_release
  CC "-Oh"
)

mcux_add_iar_configuration(
    TARGETS flash_release
    CC "-Ohs"
)

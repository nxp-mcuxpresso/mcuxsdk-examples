mcux_add_configuration(
           CC "-DBOARD_USE_PCA6416A=1"
)
mcux_add_configuration(
           CC "-DBOARD_USE_TPM=1"
)
mcux_add_configuration(
           CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/demo_apps/low_power_display/rpmsg_config.h
            examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/drivers/lpi2c/fsl_lpi2c_freertos.c
            examples/_boards/${board}/drivers/lpi2c/fsl_lpi2c_freertos.h
            examples/demo_apps/low_power_display_imx8ulp/low_power_display.h
            examples/demo_apps/low_power_display_imx8ulp/lpm.c
            examples/demo_apps/low_power_display_imx8ulp/lpm.h
            examples/demo_apps/low_power_display_imx8ulp/lcdif_support.c
            examples/demo_apps/low_power_display_imx8ulp/lcdif_support.h
            examples/_boards/${board}/demo_apps/low_power_display/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
             examples/_boards/${board}/drivers/lpi2c
             examples/demo_apps/low_power_display_imx8ulp
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
    AS "-DNO_CRP"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)

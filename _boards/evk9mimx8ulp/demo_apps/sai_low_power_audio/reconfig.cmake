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
    SOURCES examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio//./hardware_init.c
            examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio//./app.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/rsc_table.c
            examples/_boards/evkmimx8ulp/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio/srtm_config.h
            examples/_boards/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.c
            examples/_boards/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            examples/_boards/evkmimx8ulp/FreeRTOSConfigBoard.h
            examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio/rpmsg_config.h
            examples/_boards/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.c
            examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio/hardware_init.c
            examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio/app.h
            examples/_boards/evkmimx8ulp/app_srtm.c
            examples/_boards/evkmimx8ulp/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp
             middleware/multicore/remoteproc
             examples/_boards/evkmimx8ulp/demo_apps/sai_low_power_audio
             examples/_boards/evkmimx8ulp/drivers/lpi2c
)

mcux_add_macro(
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/gcc/MIMX8UD5xxxxx_ram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/iar/MIMX8UD5xxxxx_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/evkmimx8ulp/gcc/MIMX8UD7xxxxx_cm33_lpa_ram.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/evkmimx8ulp/iar/MIMX8UD7xxxxx_cm33_lpa_ram.icf
)

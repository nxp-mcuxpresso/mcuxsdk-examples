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
    SOURCES ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio//./hardware_init.c
            ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio//./app.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/rsc_table.c
            ${board_root}/evkmimx8ulp/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio/srtm_config.h
            ${board_root}/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.c
            ${board_root}/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/FreeRTOSConfig.h
            ${board_root}/evkmimx8ulp/FreeRTOSConfigBoard.h
            ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio/rpmsg_config.h
            ${board_root}/${board}/demo_apps/sai_low_power_audio/sai_low_power_audio.c
            ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio/hardware_init.c
            ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio/app.h
            ${board_root}/evkmimx8ulp/app_srtm.c
            ${board_root}/evkmimx8ulp/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp
             middleware/multicore/remoteproc
             ${board_root}/evkmimx8ulp/demo_apps/sai_low_power_audio
             ${board_root}/evkmimx8ulp/drivers/lpi2c
)

mcux_add_macro(
    AS "-DNO_CRP"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX8ULP/MIMX8UD5/gcc/MIMX8UD5xxxxx_ram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX8ULP/MIMX8UD5/iar/MIMX8UD5xxxxx_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/evkmimx8ulp/gcc/MIMX8UD7xxxxx_cm33_lpa_ram.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/evkmimx8ulp/iar/MIMX8UD7xxxxx_cm33_lpa_ram.icf
)

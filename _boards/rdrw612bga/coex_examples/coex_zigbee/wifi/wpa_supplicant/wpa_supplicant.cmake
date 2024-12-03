if(${CONFIG_WIFI} AND ${CONFIG_WPA_SUPPLICANT})
# wpa_supplicant
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_support.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/usb_api.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.h
            middleware/wifi_nxp/example/common/usb/usb_host_msd/host_msd_fatfs.c
            middleware/wifi_nxp/example/common/usb/usb_host_msd/ffconf.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wifi_nxp/example/common/usb/usb_host_msd
)

mcux_add_macro(
    CC "-DSDK_OS_FREE_RTOS\
       -DFSL_OSA_TASK_ENABLE=1\
       -DMCUX_ENABLE_TRNG_AS_ENTROPY_SEED\
       -DMBEDTLS_MCUX_ELS_PKC_API\
       -DMBEDTLS_MCUX_USE_PKC\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"wpa_supp_els_pkc_mbedtls_config.h\\\"\
       -DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\"\
       -DLWIP_HOOK_FILENAME=\\\"lwip_default_hooks.h\\\""
)
endif()
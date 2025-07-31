
mcux_add_macro(
    CC "-DSCMI_LMM_POWER_CHANGE_PROCESSED=1 \
        -DSRTM_NETC_SERVICE_USED=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
    ${board_root}/${board}/demo_apps/netc_share/cm33_core1/lwip_netc_port.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
    ${board_root}/${board}/demo_apps/netc_share/cm33_core1
)

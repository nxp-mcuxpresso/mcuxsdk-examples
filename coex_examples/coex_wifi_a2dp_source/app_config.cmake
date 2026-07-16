# coex_wifi_a2dp_source application
set(CONFIG_WIFI           1)
set(CONFIG_BLE            1)
set(CONFIG_OT             0)

# WPA_SUPPLICANT depends on WIFI - only enable if WIFI is enabled
if(CONFIG_WIFI)
    set(CONFIG_WPA_SUPPLICANT 1)
else()
    set(CONFIG_WPA_SUPPLICANT 0)
endif()

# coex defines
target_compile_definitions(${MCUX_SDK_PROJECT_NAME}
    PRIVATE
    CONFIG_WIFI_BLE_COEX_APP=${CONFIG_WIFI}
    CONFIG_DISABLE_BLE=!${CONFIG_BLE}
    CONFIG_OT_CLI=${CONFIG_OT}
    CONFIG_WPA_SUPP_MBEDTLS=${CONFIG_WPA_SUPPLICANT}
)

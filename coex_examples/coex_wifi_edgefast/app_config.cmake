# coex_wifi_edgefast applications
set(CONFIG_WIFI           1)
set(CONFIG_BLE            1)
set(CONFIG_OT             0)
set(CONFIG_WPA_SUPPLICANT 1)

# coex defines
target_compile_definitions(${MCUX_SDK_PROJECT_NAME}
    PRIVATE
    CONFIG_WIFI_BLE_COEX_APP=${CONFIG_WIFI}
    CONFIG_DISABLE_BLE=!${CONFIG_BLE}
    CONFIG_OT_CLI=${CONFIG_OT}
    CONFIG_WPA_SUPP_MBEDTLS=${CONFIG_WPA_SUPPLICANT}
)
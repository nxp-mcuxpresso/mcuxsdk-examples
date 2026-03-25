# ncp coex applications
# Enable WIFI, BLE or OT functions
set(CONFIG_NCP_WIFI           1)
set(CONFIG_NCP_BLE            0)
set(CONFIG_NCP_OT             0)
# Select the ncp interface to use
set(CONFIG_NCP_UART           1)
set(CONFIG_NCP_SPI            0)
set(CONFIG_NCP_USB            0)
set(CONFIG_NCP_SDIO           0)

# WPA_SUPPLICANT depends on NCP_WIFI - only enable if NCP_WIFI is enabled
if(CONFIG_NCP_WIFI)
    set(CONFIG_WPA_SUPPLICANT 1)
else()
    set(CONFIG_WPA_SUPPLICANT 0)
endif()

# ncp coex definitions
target_compile_definitions(${MCUX_SDK_PROJECT_NAME}
    PRIVATE
    CONFIG_NCP_WIFI=${CONFIG_NCP_WIFI}
    CONFIG_NCP_BLE=${CONFIG_NCP_BLE}
    CONFIG_NCP_OT=${CONFIG_NCP_OT}
    CONFIG_NCP_UART=${CONFIG_NCP_UART}
    CONFIG_NCP_SPI=${CONFIG_NCP_SPI}
    CONFIG_NCP_USB=${CONFIG_NCP_USB}
    CONFIG_NCP_SDIO=${CONFIG_NCP_SDIO}
    CONFIG_WPA_SUPP_MBEDTLS=${CONFIG_WPA_SUPPLICANT}
)

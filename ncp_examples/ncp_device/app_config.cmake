# ncp coex applications
# Enable WIFI, BLE or OT functions
mcux_set_variable(CONFIG_NCP_WIFI           1)
mcux_set_variable(CONFIG_NCP_BLE            0)
mcux_set_variable(CONFIG_NCP_OT             0)
# Select the ncp interface to use
mcux_set_variable(CONFIG_NCP_UART           1)
mcux_set_variable(CONFIG_NCP_SPI            0)
mcux_set_variable(CONFIG_NCP_USB            0)
mcux_set_variable(CONFIG_NCP_SDIO           0)
# Eanble wpa supplicant, only valid when WIFI is enabled
mcux_set_variable(CONFIG_WPA_SUPPLICANT     1)

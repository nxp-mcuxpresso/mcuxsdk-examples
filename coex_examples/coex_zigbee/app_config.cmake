# coex_zigbee applications
mcux_set_variable(CONFIG_BLE            1)
mcux_set_variable(CONFIG_ZIGBEE         1)
mcux_set_variable(CONFIG_WIFI           1)
if(${CONFIG_WIFI})
mcux_set_variable(CONFIG_WPA_SUPPLICANT 1)
endif()
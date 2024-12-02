mcux_set_variable(NXP_ZB_BASE ${SdkRootDirPath}/middleware/wireless/zigbee)
mcux_set_variable(PDUMCONFIG  ${NXP_ZB_BASE}/tools/PDUMConfig/Source/PDUMConfig.py)
mcux_set_variable(ZPSCONFIG   ${NXP_ZB_BASE}/tools/ZPSConfig/Source/ZPSConfig.py)
mcux_set_variable(OTA_TOOL    ${NXP_ZB_BASE}/tools/OTA/nxpzbota.py)
mcux_set_variable(ZPSCFG      ${NXP_ZB_BASE}/examples/zigbee_router/src/router.zpscfg)

mcux_set_variable(ENCRYPTION_KEY 1234567890ABCDEFA1B2C3D4E5F6F1B4)
mcux_set_variable(MANUFACTURER_CODE 0x1037)
mcux_set_variable(OTA_IMAGE_TYPE 0x0003)

mcux_set_variable(CONFIG_ZB_ROUTER_SINGLE_CHANNEL "12")
mcux_set_variable(CONFIG_ZB_ROUTER_TRACE_APP 1)
mcux_set_variable(CONFIG_ZB_ROUTER_TRACE_ZCL 1)

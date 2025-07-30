mcux_set_variable(NXP_ZB_BASE ${SdkRootDirPath}/middleware/wireless/zigbee CACHE PATH "NXP MCUXPRESSO Zigbee base")
mcux_set_variable(PDUMCONFIG  ${NXP_ZB_BASE}/tools/PDUMConfig/Source/PDUMConfig.py CACHE PATH "Zigbee PDUMConfig")
mcux_set_variable(ZPSCONFIG   ${NXP_ZB_BASE}/tools/ZPSConfig/Source/ZPSConfig.py CACHE PATH "Zigbee ZPSConfig")
mcux_set_variable(OTA_TOOL    ${NXP_ZB_BASE}/tools/OTA/nxpzbota.py CACHE PATH "OTA nxpzbota")
mcux_set_variable(MEMSIZE     ${NXP_ZB_BASE}/tools/memsize2/memsize.py)

mcux_set_variable(ENCRYPTION_KEY 1234567890ABCDEFA1B2C3D4E5F6F1B4)
mcux_set_variable(MANUFACTURER_CODE 0x1037)
mcux_set_variable(OTA_IMAGE_TYPE 0x0003)

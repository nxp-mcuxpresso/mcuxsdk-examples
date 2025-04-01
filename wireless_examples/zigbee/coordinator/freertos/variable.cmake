mcux_set_variable(NXP_ZB_BASE ${SdkRootDirPath}/middleware/wireless/zigbee)
mcux_set_variable(PDUMCONFIG  ${NXP_ZB_BASE}/tools/PDUMConfig/Source/PDUMConfig.py)
mcux_set_variable(ZPSCONFIG   ${NXP_ZB_BASE}/tools/ZPSConfig/Source/ZPSConfig.py)
mcux_set_variable(ZPSCFG      ${NXP_ZB_BASE}/examples/zigbee_coordinator/src/coordinator.zpscfg)
mcux_set_variable(MEMSIZE     ${NXP_ZB_BASE}/tools/memsize2/memsize.py)

mcux_set_variable(CONFIG_ZB_COORD_SINGLE_CHANNEL "12")
mcux_set_variable(CONFIG_ZB_COORD_TRACE_APP 0)
mcux_set_variable(CONFIG_ZB_COORD_TRACE_ZCL 0)

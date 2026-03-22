mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_PREINCLUDE boards/mcxw70/connectivity_test CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test preinclude relative path")
mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_USE_NBU 1 CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test USE_NBU")
mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_HDI_MODE 1 CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test HDI_MODE")

mcux_add_configuration(
    CC "    -DPLATFORM_TM_CLK_FREQ=16000000\
            -DPLATFORM_TM_STAMP_CLK_FREQ=16000000"
)

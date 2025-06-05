# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_PREINCLUDE boards/mcxw72/connectivity_test CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test preinclude relative path")

# USE_NBU=1
mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_USE_NBU 1 CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test USE_NBU")

# HDI_MODE = 0
mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_HDI_MODE 0 CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test HDI_MODE")

mcux_add_macro(ECC_RAM_STCM8_INIT=1)

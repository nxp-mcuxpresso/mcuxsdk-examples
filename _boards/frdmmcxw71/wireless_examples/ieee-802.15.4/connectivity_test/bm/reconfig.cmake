# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_PREINCLUDE boards/k32w1_mcxw71/connectivity_test CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test preinclude relative path")

# USE_NBU=1
mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_USE_NBU 1 CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test USE_NBU")

# HDI_MODE = 0
mcux_set_variable(NXP_IEEE_802_15_4_CONN_TEST_HDI_MODE 0 CACHE PATH "NXP MCUXPRESSO ieee-802.15.4 connectivity_test HDI_MODE")

# The NBU will send during initialization more messages than the existing pool size so connectivity_test application will deadlock
# waiting for the last packet. To fix this issue we set the Framework Platform to process the incomming RPMSGs in IRQ freeing the
# in-flight packages and removing the dead-lock.
mcux_add_configuration(CC "-DgPlatformIcsUseWorkqueueRxProcessing_d=0")

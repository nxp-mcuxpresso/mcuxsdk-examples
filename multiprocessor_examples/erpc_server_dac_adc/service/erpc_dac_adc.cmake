#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

mcux_add_include(
    INCLUDES .
)
mcux_add_source(
    SOURCES erpc_dac_adc_server.cpp
            erpc_dac_adc_server.hpp
            erpc_dac_adc_common.h
            erpc_dac_adc_common.hpp
            erpc_dac_adc_interface.hpp
            erpc_dac_adc_interface.cpp
            c_erpc_dac_adc_server.h
            c_erpc_dac_adc_server.cpp
            erpc_dac_adc_server.hpp
            erpc_dac_adc_server.cpp
            erpc_dac_adc.erpc
            __init__.py
            erpc_dac_adc/*.*
)

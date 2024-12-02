#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_boards/${board}/tfm_examples/reconfig.cmake OPTIONAL)

mcux_add_source(
        SOURCES 
		cm33_core0/hardware_init.c
		cm33_core0/pin_mux.c
		cm33_core0/pin_mux.h
		cm33_core0/app.h
)

mcux_add_include(
    INCLUDES cm33_core0/
)

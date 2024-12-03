#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES dcdc_framework.c
)

mcux_add_include(
    INCLUDES .
             ${core_id}
)

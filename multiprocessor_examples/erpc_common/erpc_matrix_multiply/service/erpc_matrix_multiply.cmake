#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

mcux_add_include(
    INCLUDES .
)
mcux_add_source(
    SOURCES __init__.py
            erpc_matrix_multiply/*.*
)

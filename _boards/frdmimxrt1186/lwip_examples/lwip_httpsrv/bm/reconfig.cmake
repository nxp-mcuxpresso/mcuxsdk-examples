# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DSDK_NETC_USED=1"
)

include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/reconfig.cmake OPTIONAL)

# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${CMAKE_CURRENT_LIST_DIR}/../../common/enet/reconfig.cmake)

mcux_add_armgcc_configuration(TARGETS debug psram_debug CC "-Og")
mcux_remove_armgcc_configuration(TARGETS debug psram_debug CC "-O0")
mcux_add_iar_configuration(TARGETS debug psram_debug CC "-Oh")
mcux_remove_iar_configuration(TARGETS debug psram_debug CC "-On")

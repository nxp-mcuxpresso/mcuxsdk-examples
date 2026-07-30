# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${CMAKE_CURRENT_LIST_DIR}/../../common/enet/reconfig.cmake)

mcux_add_armgcc_configuration(TARGETS debug psram_debug CC "-Os")
mcux_remove_armgcc_configuration(TARGETS debug psram_debug CC "-O0")
mcux_add_iar_configuration(TARGETS debug psram_debug psram_release CC "-Ohz")
mcux_remove_iar_configuration(TARGETS debug psram_debug psram_release CC "-On")

mcux_add_macro(CC "-DHAL_UART_TRANSFER_MODE=1")

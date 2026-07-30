# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x1000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x1000"
)

mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "psram_debug"
    "psram_release"
    "psram_txt_debug"
    "psram_txt_release"
)
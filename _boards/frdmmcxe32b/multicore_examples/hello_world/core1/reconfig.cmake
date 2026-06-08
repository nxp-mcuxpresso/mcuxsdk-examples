#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Override heap size for the secondary core (M7_1)
mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=0x2000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x600"
)

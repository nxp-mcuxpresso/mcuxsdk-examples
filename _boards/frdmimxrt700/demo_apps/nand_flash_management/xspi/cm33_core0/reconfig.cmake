# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000 \
             __heap_size__=0x2000"
)

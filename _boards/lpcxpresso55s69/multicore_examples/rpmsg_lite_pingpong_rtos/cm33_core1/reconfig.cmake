#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# remove generic set of heap size
mcux_remove_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x2000"
)

# Decrease heap size so it will fit into m_data region
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x1000"
)

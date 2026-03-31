# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/mcxw72_nbu_dyn_full.bin
)

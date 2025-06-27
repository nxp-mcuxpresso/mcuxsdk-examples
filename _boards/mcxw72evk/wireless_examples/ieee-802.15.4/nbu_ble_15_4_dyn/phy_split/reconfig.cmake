# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/mcxw72_nbu_ble_15_4_dyn.bin
)

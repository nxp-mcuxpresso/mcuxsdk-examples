# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS xspi_nor_release
            xspi_nor_psram_release
            psram_release
            psram_txt_release
    CC "-DLFS_NO_ASSERT"
)

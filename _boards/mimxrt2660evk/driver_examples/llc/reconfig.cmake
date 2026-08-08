# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# The LLC test region on MIMXRT2660-EVK is the on-board APS512XXN Xccela PSRAM
# behind XSPI1. It is NOT brought up by the Boot ROM on the default targets, so
# the board port initialises it in software via xspi_hyper_ram_init(). Reuse
# the silicon-validated bring-up implementation from the xspi/psram
# polling_transfer driver example instead of duplicating it here.
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/xspi/psram/polling_transfer/xspi_psram_ops.c
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/xspi/psram/polling_transfer
)

# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

# NOTE: literal device name — the RT2660 JLinkDevices patch (installed at
# %APPDATA%/SEGGER/JLinkDevices/NXP/iMXRT2660/) defines MIMXRT2660_CM85 with the
# XSPI flash loaders and the SoC connect/reset script; that name is silicon-
# validated. Kconfig MCUX_TOOLCHAIN_JLINK_CPU_IDENTIFIER currently says
# MIMXRT2663_M85, which J-Link does not know — switch to the Kconfig variable
# once the device naming is reconciled.
board_runner_args(jlink "--device=MIMXRT2660_CM85")
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

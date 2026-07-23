# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# HDI_MODE = 0
mcux_set_variable(HDI_MODE 0 CACHE PATH "HDI_MODE")

# FPGA_TARGET = 0
mcux_set_variable(FPGA_TARGET 0 CACHE PATH "FPGA_TARGET")

# Enable assert on release build.
include(${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu/enable_assert_release.cmake)


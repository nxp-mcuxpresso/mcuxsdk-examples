# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1")

mcux_add_source(
  BASE_PATH ${SdkRootDirPath} SOURCES
  ${board_root}/${board}/driver_examples/enet/common/hardware_init.c
  ${board_root}/${board}/driver_examples/enet/common/app.h)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES
                 ${board_root}/${board}/driver_examples/enet/common)

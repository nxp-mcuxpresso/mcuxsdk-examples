# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# CHECKSUM_GEN_IP=1: RT2660 ENET_QOS has no HW TX checksum offload, so lwIP computes checksums.
mcux_add_macro(CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DCHECKSUM_GEN_IP=1")

mcux_add_source(
  BASE_PATH ${SdkRootDirPath} SOURCES
  ${board_root}/${board}/lwip_examples/common/enet_qos/hardware_init.c
  ${board_root}/${board}/lwip_examples/common/enet_qos/app.h)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES
                 ${board_root}/${board}/lwip_examples/common/enet_qos)

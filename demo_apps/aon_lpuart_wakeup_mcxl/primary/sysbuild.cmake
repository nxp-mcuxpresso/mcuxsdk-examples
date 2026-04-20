#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

externalzephyrproject_add(
  APPLICATION
  aon_lpuart_wakeup_mcxl_secondary
  SOURCE_DIR
  ${APP_DIR}/../secondary
  board
  ${SB_CONFIG_secondary_board}
  core_id
  ${SB_CONFIG_secondary_core_id}
  config
  ${SB_CONFIG_secondary_config}
  toolchain
  ${SB_CONFIG_secondary_toolchain})

# Let's build the secondary application first
add_dependencies(${DEFAULT_IMAGE} aon_lpuart_wakeup_mcxl_secondary)

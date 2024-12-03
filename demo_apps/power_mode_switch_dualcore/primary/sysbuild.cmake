#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
    APPLICATION power_mode_switch_secondary
    SOURCE_DIR  ${APP_DIR}/../secondary
    board ${SB_CONFIG_secondary_board}
    core_id ${SB_CONFIG_secondary_core_id}
    config ${SB_CONFIG_secondary_config}
    toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the secondary application first
add_dependencies(${DEFAULT_IMAGE} power_mode_switch_secondary)

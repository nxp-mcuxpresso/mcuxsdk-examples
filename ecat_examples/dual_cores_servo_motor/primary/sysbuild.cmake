#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalZephyrProject_Add(
        APPLICATION dual_cores_ecat_servo_motor_secondary_core
        SOURCE_DIR  ${APP_DIR}/../secondary
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the secondary application first
add_dependencies(${DEFAULT_IMAGE} dual_cores_ecat_servo_motor_secondary_core)
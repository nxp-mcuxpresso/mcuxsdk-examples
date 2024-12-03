#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
        APPLICATION secure_rgpio_s
        SOURCE_DIR  ${APP_DIR}/../secure_rgpio_s
        board ${board}
        core_id ${core_id}
        config ${config}
        toolchain ${toolchain}
)

# Let's build the secure application first
add_dependencies(${DEFAULT_IMAGE} secure_rgpio_s)
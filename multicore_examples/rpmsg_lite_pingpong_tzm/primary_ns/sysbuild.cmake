#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
        APPLICATION rpmsg_lite_pingpong_tzm_secondary_core_s
        SOURCE_DIR  ${APP_DIR}/../secondary_s
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

ExternalMCUXProject_Add(
        APPLICATION rpmsg_lite_pingpong_tzm_primary_core_s
        SOURCE_DIR  ${APP_DIR}/../primary_s
        board ${board}
        core_id ${core_id}
        config ${config}
        toolchain ${toolchain}
)

# Let's build the secondary application first
add_dependencies(rpmsg_lite_pingpong_tzm_primary_core_s rpmsg_lite_pingpong_tzm_secondary_core_s)

# Let's build the primary_s application second
add_dependencies(${DEFAULT_IMAGE} rpmsg_lite_pingpong_tzm_primary_core_s)

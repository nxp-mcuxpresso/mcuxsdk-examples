#
# Copyright 2025, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalZephyrProject_Add(
        APPLICATION rpmsg_lite_pingpong_dsp_hifi4
        SOURCE_DIR  ${APP_DIR}/../hifi4
        board ${SB_CONFIG_dsp_board}
        core_id ${SB_CONFIG_primary_dsp_core_id}
        config ${SB_CONFIG_dsp_config}
        toolchain ${SB_CONFIG_dsp_toolchain}
)

ExternalMCUXProject_Add(
        APPLICATION rpmsg_lite_pingpong_dsp_hifi1
        SOURCE_DIR  ${APP_DIR}/../hifi1
        board ${SB_CONFIG_dsp_board}
        core_id ${SB_CONFIG_secondary_dsp_core_id}
        config ${SB_CONFIG_dsp_config}
        toolchain ${SB_CONFIG_dsp_toolchain}
)

ExternalZephyrProject_Add(
        APPLICATION rpmsg_lite_pingpong_dsp_secondary_core
        SOURCE_DIR  ${APP_DIR}/../secondary
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the hifi4 application first
add_dependencies(rpmsg_lite_pingpong_dsp_secondary_core rpmsg_lite_pingpong_dsp_hifi1)

add_dependencies(${DEFAULT_IMAGE} rpmsg_lite_pingpong_dsp_hifi4)

add_dependencies(${DEFAULT_IMAGE} rpmsg_lite_pingpong_dsp_secondary_core)

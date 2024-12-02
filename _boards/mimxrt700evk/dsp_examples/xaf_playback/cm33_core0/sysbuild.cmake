#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalZephyrProject_Add(
        APPLICATION xaf_playback_hifi4
        SOURCE_DIR  ${APP_DIR}/../dsp
        board ${SB_CONFIG_dsp_board}
        core_id ${SB_CONFIG_primary_dsp_core_id}
        config ${SB_CONFIG_dsp_config}
        toolchain ${SB_CONFIG_dsp_toolchain}
)

ExternalMCUXProject_Add(
        APPLICATION xaf_playback_hifi1
        SOURCE_DIR  ${APP_DIR}/../dsp
        board ${SB_CONFIG_dsp_board}
        core_id ${SB_CONFIG_secondary_dsp_core_id}
        config ${SB_CONFIG_dsp_config}
        toolchain ${SB_CONFIG_dsp_toolchain}
)

ExternalZephyrProject_Add(
        APPLICATION xaf_playback_cm33_core1
        SOURCE_DIR  ${APP_DIR}/../cm_core1
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the hifi4 application first
add_dependencies(xaf_playback_cm33_core1 xaf_playback_hifi1)

add_dependencies(${DEFAULT_IMAGE} xaf_playback_hifi4)

add_dependencies(${DEFAULT_IMAGE} xaf_playback_cm33_core1)
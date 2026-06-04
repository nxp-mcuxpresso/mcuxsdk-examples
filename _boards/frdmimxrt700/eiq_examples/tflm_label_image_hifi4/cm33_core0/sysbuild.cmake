#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalZephyrProject_Add(
        APPLICATION tflm_label_image_hifi4_hifi4
        SOURCE_DIR  ${APP_DIR}/../hifi4
        board ${SB_CONFIG_dsp_board}
        core_id ${SB_CONFIG_primary_dsp_core_id}
        config ${SB_CONFIG_dsp_config}
        toolchain ${SB_CONFIG_dsp_toolchain}
)

add_dependencies(${DEFAULT_IMAGE} tflm_label_image_hifi4_hifi4)

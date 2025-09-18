#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause


ExternalMCUXProject_Add(
        APPLICATION tflm_cifar10_hifi1_hifi1
        SOURCE_DIR  ${APP_DIR}/../hifi1
        board ${SB_CONFIG_dsp_board}
        core_id ${SB_CONFIG_secondary_dsp_core_id}
        config ${SB_CONFIG_dsp_config}
        toolchain ${SB_CONFIG_dsp_toolchain}
)

ExternalZephyrProject_Add(
        APPLICATION tflm_cifar10_hifi1_cm33_core1
        SOURCE_DIR  ${APP_DIR}/../cm_core1
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the hifi1 application first
add_dependencies(tflm_cifar10_hifi1_cm33_core1 tflm_cifar10_hifi1_hifi1)

add_dependencies(${DEFAULT_IMAGE} tflm_cifar10_hifi1_cm33_core1)

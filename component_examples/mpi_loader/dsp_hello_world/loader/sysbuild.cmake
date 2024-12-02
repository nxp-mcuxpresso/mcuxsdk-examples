#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
    APPLICATION mpi_loader_dsp_hello_world
    SOURCE_DIR  ${APP_DIR}/../dsp_hello_world
    board ${SB_CONFIG_dsp_board}
    core_id ${SB_CONFIG_dsp_core_id}
    config ${SB_CONFIG_dsp_config}
    toolchain ${SB_CONFIG_dsp_toolchain}
)

# Let's build the app application first
add_dependencies(${DEFAULT_IMAGE} mpi_loader_dsp_hello_world)

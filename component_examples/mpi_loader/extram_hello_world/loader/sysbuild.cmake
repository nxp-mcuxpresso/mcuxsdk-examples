#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
    APPLICATION mpi_loader_extram_hello_world
    SOURCE_DIR  ${APP_DIR}/../extram_hello_world
    board ${board}
    core_id ${core_id}
    config ${config}
    toolchain ${toolchain}
)

# Let's build the app application first
add_dependencies(${DEFAULT_IMAGE} mpi_loader_extram_hello_world)

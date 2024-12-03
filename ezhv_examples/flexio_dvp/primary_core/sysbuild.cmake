# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples/flexio_dvp/${core_id}/sysbuild.cmake OPTIONAL)

ExternalZephyrProject_Add(
        APPLICATION ezhv_flexio_dvp_ezhv
        SOURCE_DIR  ${APP_DIR}/../ezhv
        board ${SB_CONFIG_ezhv_board}
        core_id ${SB_CONFIG_ezhv_core_id}
        config ${SB_CONFIG_ezhv_config}
        toolchain ${SB_CONFIG_ezhv_toolchain}
)

# Build the ezhv project first
add_dependencies(${DEFAULT_IMAGE} ezhv_flexio_dvp_ezhv)

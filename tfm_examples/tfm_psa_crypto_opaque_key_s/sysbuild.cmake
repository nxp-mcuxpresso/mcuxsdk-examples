#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
        APPLICATION tfm_s_crypto_clients
        SOURCE_DIR  ${APP_DIR}/../../../middleware/tfm/tf-m/mcux/tfm_s_crypto_clients
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)
# Let's build the tfm_s_crypto_clients application second
add_dependencies(${DEFAULT_IMAGE} tfm_s_crypto_clients)

#file to provide additonal configurations to secondary projects, e.g: tfm profile other than default
include (${SdkRootDirPath}/${board_root}/${board}/tfm_examples/tfm_psa_crypto_opaque_key/tfm_psa_crypto_opaque_key_s/sysbuild.cmake OPTIONAL)
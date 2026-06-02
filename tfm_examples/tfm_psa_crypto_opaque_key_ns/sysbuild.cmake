#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

ExternalMCUXProject_Add(
        APPLICATION tfm_psa_crypto_opaque_key_s
        SOURCE_DIR  ${APP_DIR}/../tfm_psa_crypto_opaque_key_s
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)
# Let's build the tfm_psa_crypto_opaque_key_s application second
add_dependencies(${DEFAULT_IMAGE} tfm_psa_crypto_opaque_key_s)

# Let's build the tfm_s_crypto_clients application second
add_dependencies(tfm_psa_crypto_opaque_key_s tfm_s_crypto_clients)

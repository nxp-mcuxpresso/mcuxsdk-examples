#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
ExternalMCUXProject_Add(
    APPLICATION el2go_csr_s
    SOURCE_DIR  ${APP_DIR}/../el2go_csr_s
)

add_dependencies(${DEFAULT_IMAGE} el2go_csr_s)

add_dependencies(el2go_csr_s tfm_s_crypto_clients)
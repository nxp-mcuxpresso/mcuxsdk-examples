#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# primary project setting
set(CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small y
    CACHE BOOL "primary configuration" FORCE)

# secondary project setting
set(tfm_s_crypto_clients_CONFIG_MCUX_COMPONENT_middleware.tfm.profile_medium n
    CACHE BOOL "secondary configuration" FORCE)

set(tfm_s_crypto_clients_CONFIG_MCUX_COMPONENT_middleware.tfm.s.protected_storage n
    CACHE BOOL "secondary configuration" FORCE)

# secondary project setting
set(tfm_s_crypto_clients_CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small y
    CACHE BOOL "secondary configuration" FORCE)

# secondary project setting
set(tfm_s_reg_suites_CONFIG_MCUX_COMPONENT_middleware.tfm.profile_small y
    CACHE BOOL "secondary configuration" FORCE)
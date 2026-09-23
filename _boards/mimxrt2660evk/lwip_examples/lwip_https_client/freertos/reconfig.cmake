# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${CMAKE_CURRENT_LIST_DIR}/../../common/enet/reconfig.cmake)

# RT2660 has no entropy source in-tree: its ELE is an S100 (no PSA crypto driver
# exists for it) and the SoC has no TRNG/RNG. Build mbedTLS software-only so the
# port does not require mbedtls_hardware_poll. This disables HW acceleration and
# HW entropy, so it is for demonstration only, not a secure TLS configuration.
mcux_add_macro(CC "-DMBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR=0")

mcux_add_armgcc_configuration(TARGETS debug CC "-Os")
mcux_remove_armgcc_configuration(TARGETS debug CC "-O0")
mcux_add_iar_configuration(TARGETS debug CC "-Ohz")
mcux_remove_iar_configuration(TARGETS debug CC "-On")

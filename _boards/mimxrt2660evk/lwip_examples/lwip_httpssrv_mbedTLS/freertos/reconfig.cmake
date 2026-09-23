# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${CMAKE_CURRENT_LIST_DIR}/../../common/enet/reconfig.cmake)

# RT2660 has no entropy source in-tree: its ELE is an S100 (no PSA crypto driver
# exists for it) and the SoC has no TRNG/RNG. Build mbedTLS software-only so the
# port does not require mbedtls_hardware_poll. This disables HW acceleration and
# HW entropy, so it is for demonstration only, not a secure TLS configuration.
mcux_add_macro(CC "-DMBEDCRYPTO_MCUX_CRYPTO_HW_ACCELERATOR=0")

# The httpssrv component defines MBEDTLS_THREADING_ALT, so threading.h needs
# threading_alt.h. Add the PSA crypto OSAL only; no HW crypto driver is pulled in.
mcux_add_source(
  BASE_PATH ${SdkRootDirPath} SOURCES
  components/psa_crypto_driver/osal/threading_alt.h
  components/psa_crypto_driver/osal/osal_mutex.h
  components/psa_crypto_driver/osal/frtos/threading_alt.c
  components/psa_crypto_driver/osal/frtos/osal_mutex.c)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath} INCLUDES
  components/psa_crypto_driver/osal
  components/psa_crypto_driver/osal/frtos)

mcux_add_armgcc_configuration(TARGETS debug CC "-Os")
mcux_remove_armgcc_configuration(TARGETS debug CC "-O0")
mcux_add_iar_configuration(TARGETS debug CC "-Ohz")
mcux_remove_iar_configuration(TARGETS debug CC "-On")

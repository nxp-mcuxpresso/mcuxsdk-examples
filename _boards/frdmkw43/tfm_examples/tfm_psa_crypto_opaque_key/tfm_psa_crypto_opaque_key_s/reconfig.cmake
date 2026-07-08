#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)

#add cc-defines
mcux_add_macro(
     CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DOCOTP_NV_COUNTERS_RAM_EMULATION=1\
       -DPSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT\
       -DOTP_NV_COUNTERS_RAM_EMULATION\
       -DTFM_FIH_PROFILE_ON\
       -DTFM_FIH_PROFILE_MEDIUM\
       -DFIH_CFI_ALT\
       "
)

# The TF-M "profile_medium" crypto config (crypto_config_profile_medium.h)
# leaves several symmetric algorithms disabled by default. The SGI opaque-key
# example exercises AES cipher (ECB/CBC/CTR) and AEAD (CCM/GCM), so the
# corresponding PSA_WANT_ALG_* symbols must be enabled. Without them the SGI
# transparent cipher/AEAD drivers cannot resolve the mode and return
# PSA_ERROR_NOT_SUPPORTED (-134) at runtime.
# Note: CBC_NO_PADDING and CMAC are already enabled via
# CONFIG_MCUX_COMPONENT_middleware.tfm.enable_crypto_cipher_module.
mcux_add_macro(
     CC "-DPSA_WANT_ALG_ECB_NO_PADDING=1\
       -DPSA_WANT_ALG_CTR=1\
       -DPSA_WANT_ALG_GCM=1\
       "
)

#mdk configurations:
mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
    CX "-Oz"
)

#armgcc configurations
mcux_remove_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS debug release 
    CC "-DDEBUG"
)
mcux_add_macro(
    TOOLCHAINS armgcc  iar mdk
    TARGETS debug release
    CC "-DNDEBUG"
)
#armgcc configurations
mcux_remove_armgcc_configuration(
    TARGETS debug release
    CC "-O0"
    CX "-O0"
)

mcux_remove_armgcc_configuration(
    TARGETS debug release
    CC "-O1"
    CX "-O1"
)
mcux_add_armgcc_configuration(
    TARGETS debug release
    CC "-Os"
    CX "-Os"
)

#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug
    CX "--diag_suppress=Pa082,Pa050"
    CC "--diag_suppress=Pa082,Pa050 -On"
)

mcux_remove_iar_configuration(
    TARGETS debug release
    CX "-Oh"
    CC "-Oh --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling"
)

mcux_add_iar_configuration(
    TARGETS debug release
    CC "-Ohz"
)


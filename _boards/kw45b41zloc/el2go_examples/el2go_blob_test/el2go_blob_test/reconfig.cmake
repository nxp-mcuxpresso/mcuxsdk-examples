# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1"
)

mcux_add_macro(
    CC "PSA_WANT_ALG_GCM\
        PSA_WANT_ALG_SHA_1"
)

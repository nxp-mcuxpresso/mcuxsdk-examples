# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/el2go_examples/el2go_blob_test/el2go_blob_test/peripherals.c
            ${board_root}/${board}/el2go_examples/el2go_blob_test/el2go_blob_test/peripherals.h
)

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

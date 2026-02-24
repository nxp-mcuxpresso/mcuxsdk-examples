# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/el2go_examples/el2go_import_blob/el2go_import_blob/${core_id}
)

# Add additional configuration
mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

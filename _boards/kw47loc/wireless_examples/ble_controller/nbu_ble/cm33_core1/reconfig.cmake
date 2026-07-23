# Copyright 2025-2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ble_controller/boards/kw47evk_nbu/nbu_ble/
    SOURCES app_preinclude.h
	PREINCLUDE TRUE
)

# Enable assert on release build.
include(${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu/enable_assert_release.cmake)

mcux_add_source(
    SOURCES ../readme.txt
)

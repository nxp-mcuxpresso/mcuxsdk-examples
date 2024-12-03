# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_boards/lpcxpresso55s69/${core_id}/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/flexcomm_spi/lvgl_support.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

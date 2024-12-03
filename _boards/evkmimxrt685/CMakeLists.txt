# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
mcux_add_cmakelists(${SdkRootDirPath}/examples/_boards/evkmimxrt685/${core_id} OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/evkmimxrt685/${core_id}/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/flexcomm_spi/lvgl_support.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

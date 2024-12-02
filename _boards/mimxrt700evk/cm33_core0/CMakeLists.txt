# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/ext_flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_common/components/flash_config.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/dsp/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/multicore/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/pmic/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/ezhv/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/vglite_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt700/lvgl_support.cmake)

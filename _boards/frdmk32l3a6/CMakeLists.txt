# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/${board_root}/frdmk32l3a6/${core_id}/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

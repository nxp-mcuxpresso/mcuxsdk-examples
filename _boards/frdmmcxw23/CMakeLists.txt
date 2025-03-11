# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/${board_root}/frdmmcxw23/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/wireless/mcxw23/prjseg.cmake)
mcux_add_cmakelists(${SdkRootDirPath}/middleware/wireless/bluetooth/application/common)
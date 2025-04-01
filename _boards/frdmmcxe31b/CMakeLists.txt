# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)
include(${SdkRootDirPath}/examples/_boards/frdmmcxe31b/setting.cmake)

mcux_add_cmakelists(${CMAKE_CURRENT_LIST_DIR}/boot_header)

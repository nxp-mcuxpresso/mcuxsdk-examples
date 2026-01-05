# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
mcux_add_cmakelists(${SdkRootDirPath}/${board_root}/frdmimxrt700/${core_id} OPTIONAL)
include(${SdkRootDirPath}/${board_root}/frdmimxrt700/${core_id}/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

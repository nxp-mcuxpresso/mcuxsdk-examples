# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
mcux_add_cmakelists(${SdkRootDirPath}/examples/_boards/mimxrt700evk/${core_id} OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/mimxrt700evk/${core_id}/setting.cmake)

# shields
mcux_add_cmakelists(${SdkRootDirPath}/examples/_boards/mimxrt700evk/shields/a8974 OPTIONAL)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

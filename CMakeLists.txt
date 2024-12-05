# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

# Board cmakelist
mcux_add_cmakelists(${SdkRootDirPath}/${board_root}/${board})

# Project segment
include(${SdkRootDirPath}/examples/_common/project_segments/common/prjseg.cmake)

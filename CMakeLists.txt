# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

# Board cmakelist
mcux_add_cmakelists(${SdkRootDirPath}/examples/_boards/${board})

# Project segment
include(${SdkRootDirPath}/examples/_common/project_segments/common/prjseg.cmake)

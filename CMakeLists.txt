# Copyright 2025 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

# Board cmakelist
# External repository board, exclusive to the internal repository board
mcux_add_cmakelists(${CUSTOM_BOARD_ROOT}/${board} OPTIONAL)

# Internal repository board, exclusive to the external repository board
mcux_add_cmakelists(${SdkRootDirPath}/${board_root}/${board} OPTIONAL)

# Project segment
include(${SdkRootDirPath}/examples/_common/project_segments/common/prjseg.cmake)

# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/display_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
)

include(${SdkRootDirPath}/examples/_boards/imx95lpd5evk19/driver_examples/dpu/reconfig.cmake)

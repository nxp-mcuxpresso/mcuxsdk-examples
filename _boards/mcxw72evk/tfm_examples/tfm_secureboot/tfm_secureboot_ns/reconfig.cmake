#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)

mcux_add_armgcc_configuration(
    CC "-Wno-implicit-function-declaration"
)

mcux_add_mdk_configuration(
    CC "-Wno-implicit-function-declaration"
)

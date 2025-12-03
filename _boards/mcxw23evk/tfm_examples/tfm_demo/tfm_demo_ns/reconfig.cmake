#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)

if(CONFIG_MCUX_HW_DEVICE_ID_MCXW235)
    set(TFM_PLATFORM_LINKER_DEFINES "-DMCXW235_SERIES")
endif()
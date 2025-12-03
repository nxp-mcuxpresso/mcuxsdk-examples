#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/${board_root}/${board}/tfm_examples/reconfig.cmake OPTIONAL)

if(CONFIG_MCUX_HW_DEVICE_ID_MCXW235)
    set(TFM_PLATFORM_LINKER_DEFINES "-DMCXW235_SERIES")
endif()

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Os"
    CX "-Os"
)

#mdk configurations:
mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
    CX "-Oz"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)

mcux_add_iar_configuration(
    TARGETS debug
    CC "-Ohz"
)

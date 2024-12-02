# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)


if (CONFIG_MCUX_COMPONENT_device.utility)

    mcux_add_source(
        SOURCES
            utility/board_utility.c
            utility/board_utility.h
    )

    mcux_add_include(
        INCLUDES ./utility
    )

endif()

mcux_add_cmakelists(${SdkRootDirPath}/examples/_boards/frdmmcxw72/${core_id} OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/frdmmcxw72/${core_id}/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_mcu/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)
mcux_add_cmakelists(${SdkRootDirPath}/examples/wireless_examples/bluetooth/common)

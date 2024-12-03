# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/lpc_lcdc/lvgl_support.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_remove_iar_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER devices/LPC/LPC54000/LPC54S018M/iar/LPC54S018J4M_flash.icf
    )

    mcux_remove_armgcc_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER devices/LPC/LPC54000/LPC54S018M/gcc/LPC54S018J4M_flash.ld
    )

    mcux_remove_mdk_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER devices/LPC/LPC54000/LPC54S018M/arm/LPC54S018J4M_flash.scf
    )

    mcux_add_iar_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER devices/LPC/LPC54000/LPC54S018M/iar/LPC54S018J4M_spifi_flash.icf
    )

    mcux_add_armgcc_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER devices/LPC/LPC54000/LPC54S018M/gcc/LPC54S018J4M_spifi_flash.ld
    )

    mcux_add_mdk_linker_script(
            TARGETS debug release
            BASE_PATH ${SdkRootDirPath}
            LINKER devices/LPC/LPC54000/LPC54S018M/arm/LPC54S018J4M_spifi_flash.scf
    )
endif()

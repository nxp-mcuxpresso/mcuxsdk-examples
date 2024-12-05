# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_macro(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_MIPI)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.DPU_DI_LVDS)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.LCD_SPEC)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.LVDS_DUAL_PANEL)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.MX8_DSI_OLED2)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.MIPI2HDMI)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.LVDS2HDMI)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.LDB_DUAL_PANEL_DISABLE)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.LDB_DUAL_PANEL_ENABLE)

endif()

# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_macro(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.RK055AHD091)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.RK055MHD091A0)

endif()

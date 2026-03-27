# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_macro(
        CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
            -DSSD1963_DATA_WITDH=8\
            -DSDK_I2C_BASED_COMPONENT_USED=1\
            -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8"
    )
endif()

# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_macro(
        CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
            -DSSD1963_DATA_WITDH=8\
            -DSDK_I2C_BASED_COMPONENT_USED=1\
            -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8\
            -DDBI_USE_MIPI_PANEL=1"
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.TFT_PROTO_5)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.TFT_PROTO_5_FLEXIO)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.TFT_PROTO_5_LCDIF)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.RK055AHD091)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.RK055IQH091)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.G1120B0MIPI)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.RK055MHD091A0)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.RASPI_7INCH)

endif()

if (CONFIG_MCUX_PRJSEG_module.board.display_support.ZC143AC72MIPI)

endif()

# Copyright 202 N5XP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_macro(
        CC "-DST7796S_DATA_WITDH=16\
            -DSDK_I2C_BASED_COMPONENT_USED=1\
            -DBOARD_USE_PCAL6524=1\
            -DBOARD_USE_PCA9555=1"
    )
endif()

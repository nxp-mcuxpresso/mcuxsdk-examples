# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.board.suite)

mcux_add_armgcc_configuration(
    TARGETS debug release
    LD "-Xlinker --no-wchar-size-warning\
        -Xlinker -znoexecstack"
)

endif()

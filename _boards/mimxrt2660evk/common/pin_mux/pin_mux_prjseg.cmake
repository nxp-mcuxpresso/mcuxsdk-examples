# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
# RT2660 mimxrt2660evk pin-mux source selection (2660-specific override).
#
# Two file sets are available:
#   - pin_mux_fpga.[ch]   FPGA / pre-silicon bring-up files (this segment).
#   - pin_mux.[ch]        Post-silicon EVK files.  Selected via the standard
#                          CONFIG_MCUX_PRJSEG_module.board.pinmux_board_folder
#                          guard in examples/_common/project_segments/common/prjseg.cmake.
#
# The choice is driven entirely by prj.conf:
#   FPGA build:  CONFIG_MCUX_PRJSEG_module.board.pinmux_customize_folder=y
#   EVK  build:  CONFIG_MCUX_PRJSEG_module.board.pinmux_board_folder=y
#
# The guard below must remain the innermost `if(...)` enclosing the
# mcux_add_source / mcux_add_include calls — sdkgen's CMake parser only
# attributes files to a project segment when the enclosing condition is a
# CONFIG_MCUX_PRJSEG_* symbol.  A nested `if(RT2660_PRESILICON)` (or any other
# non-MCUX_PRJSEG condition) sets identifier=bypass and the files are dropped
# from package_raw output.
#
# RT2660_PRESILICON now only controls the -DRT2660_PRESILICON_DEVELOPMENT=1
# macro in the board CMakeLists.txt.

if (CONFIG_MCUX_PRJSEG_module.board.pinmux_customize_folder)
    mcux_add_source(
      BASE_PATH ${SdkRootDirPath}
      SOURCES
        ${board_root}/${board}/common/pin_mux/fpga/pin_mux.h
        ${board_root}/${board}/common/pin_mux/fpga/pin_mux.c
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES ${board_root}/${board}/common/pin_mux/fpga
    )
endif()

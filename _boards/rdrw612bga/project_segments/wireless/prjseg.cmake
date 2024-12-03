# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(gcc_wireless_linker_file RW61x_flash.ld)

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script.zb_mcuboot)
    mcux_set_variable(gcc_wireless_linker_file RW61x_flash_zb_mcuboot.ld)
endif()

mcux_set_variable(iar_wireless_linker_file RW61x_flash.icf)
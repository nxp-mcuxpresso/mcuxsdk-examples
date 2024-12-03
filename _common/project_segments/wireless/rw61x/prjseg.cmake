# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board_comp)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_boards/${board}/wireless_examples/components/board_comp.c
            examples/_boards/${board}/wireless_examples/components/board_comp.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_boards/${board}/wireless_examples/components
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.lowpower)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/rw61x/low_power/board_lp.c
            examples/_common/project_segments/wireless/rw61x/low_power/board_lp.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_common/project_segments/wireless/rw61x/low_power

    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.app_services)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/_common/project_segments/wireless/rw61x/app_common/app_services_init.c
    )
endif()

include(${SdkRootDirPath}/examples/_boards/${board}/project_segments/wireless/prjseg.cmake)

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script)
    mcux_remove_armgcc_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_remove_iar_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
    )
    mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
    )

    mcux_add_armgcc_linker_script(
        TARGETS debug release flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/wireless_examples
        LINKER linker/gcc/${gcc_wireless_linker_file}
    )

    mcux_add_iar_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/wireless_examples
        LINKER linker/iar/${iar_wireless_linker_file}
    )
endif()

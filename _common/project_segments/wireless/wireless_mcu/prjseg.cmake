# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.board.wireless.clock)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_boards/${board}/wireless_examples/clock_config.c
            examples/_boards/${board}/wireless_examples/clock_config.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_boards/${board}/wireless_examples
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/board.c
            examples/_common/project_segments/wireless/wireless_mcu/board.h
            examples/_boards/${board}/wireless_examples/board_platform.h
            examples/_boards/${board}/wireless_examples/pin_mux_lp.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.components)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/components/board_comp.c
            examples/_common/project_segments/wireless/wireless_mcu/components/board_comp.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu/components
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.dcdc)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/dcdc/board_dcdc.c
            examples/_common/project_segments/wireless/wireless_mcu/dcdc/board_dcdc.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu/dcdc
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/debug/board_debug.c
            examples/_common/project_segments/wireless/wireless_mcu/debug/board_debug.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu/debug
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.extflash)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/ext_flash/board_extflash.c
            examples/_common/project_segments/wireless/wireless_mcu/ext_flash/board_extflash.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu/ext_flash

    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.lowpower)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/low_power/board_lp.c
            examples/_common/project_segments/wireless/wireless_mcu/low_power/board_lp.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu/low_power
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.app_services)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/_common/project_segments/wireless/wireless_mcu/app_common/app_services_init.c
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.app_ble)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES 
            examples/_common/project_segments/wireless/wireless_mcu/app_common/app_ble_init.c
            examples/_common/project_segments/wireless/wireless_mcu/app_common/app.h

    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.app_lce)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/app_common/app_lce_init.c
            examples/_common/project_segments/wireless/wireless_mcu/app_common/app_lce_init.h
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.hw_app)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/app_common/hardware_init.c
            examples/_common/project_segments/wireless/wireless_mcu/app_common/app.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_common/project_segments/wireless/wireless_mcu/app_common
    )
endif()

include(${SdkRootDirPath}/examples/_boards/${board}/project_segments/wireless/prjseg.cmake)

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER examples/_boards/${board}/wireless_examples/linker/gcc/${gcc_wireless_linker_file}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER examples/_boards/${board}/wireless_examples/linker/iar/${iar_wireless_linker_file}
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script_ble)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/wireless_examples
        TARGETS debug release
        LINKER linker/gcc/${gcc_wireless_linker_file_ble}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/wireless_examples
        TARGETS debug release
        LINKER linker/iar/${iar_wireless_linker_file}
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.core_linker_script)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER examples/_boards/${board}/wireless_examples/linker/${core_id}/gcc/${gcc_wireless_linker_file}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER examples/_boards/${board}/wireless_examples/linker/iar/${core_id}/${iar_wireless_linker_file}
    )
endif()

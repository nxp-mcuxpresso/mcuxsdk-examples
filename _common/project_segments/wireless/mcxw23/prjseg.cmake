# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            ${board_root}/${board}/wireless_examples/board_platform.h
            ${board_root}/${board}/wireless_examples/pin_mux.h
            ${board_root}/${board}/board.c
            ${board_root}/${board}/board.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            ${board_root}/${board}/
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.components)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/mcxw23/components/board_comp.c
            examples/_common/project_segments/wireless/mcxw23/components/board_comp.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/mcxw23/components
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.hw_app)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/mcxw23/app_common/hardware_init.c
            examples/_common/project_segments/wireless/wireless_mcu/app_common/app.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_common/project_segments/wireless/wireless_mcu/app_common
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.app_services)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/_common/project_segments/wireless/wireless_mcu/app_common/app_services_init.c
    )
endif()

mcux_set_variable(gcc_wireless_linker_file_ble connectivity_ble.ld)
mcux_set_variable(iar_wireless_linker_file connectivity.icf)
mcux_set_variable(arm_wireless_linker_file connectivity.scf)

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script_ble)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/mcxw23
        TARGETS debug release
        LINKER linker/gcc/${gcc_wireless_linker_file_ble}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/mcxw23
        TARGETS debug release
        LINKER linker/iar/${iar_wireless_linker_file}
    )

    mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
    )

    mcux_add_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/mcxw23
        TARGETS debug release
        LINKER linker/arm/${arm_wireless_linker_file}
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.app_ble)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_common/project_segments/wireless/mcxw23/app_common/app_ble_init.c
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_common/project_segments/wireless/wireless_mcu/app_common
)
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.extflash)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/mcxw23/ext_flash/board_extflash.c
            examples/_common/project_segments/wireless/mcxw23/ext_flash/board_extflash.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/mcxw23/ext_flash
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.lowpower)
# Not implemented for MCXW23 but cmake definition needed for ecosystem build.
endif()
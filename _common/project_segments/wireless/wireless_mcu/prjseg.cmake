# Copyright 2024-2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.board.wireless.clock)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            ${board_root}/${board}/wireless_examples/clock_config.c
            ${board_root}/${board}/wireless_examples/clock_config.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES ${board_root}/${board}/wireless_examples
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/board.c
            examples/_common/project_segments/wireless/wireless_mcu/board.h
            ${board_root}/${board}/wireless_examples/board_platform.h
            ${board_root}/${board}/wireless_examples/pin_mux_lp.h
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

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu)
    mcux_add_source(
        TOOLCHAINS armgcc iar
        BASE_PATH ${SdkRootDirPath}
        SOURCES
            examples/_common/project_segments/wireless/wireless_mcu/debug/board_debug_nbu.c
            examples/_common/project_segments/wireless/wireless_mcu/debug/board_debug_nbu.h
            examples/_common/project_segments/wireless/wireless_mcu/debug/readme.md
    )
    mcux_add_include(
        TOOLCHAINS armgcc iar
        BASE_PATH ${SdkRootDirPath}
        INCLUDES
            examples/_common/project_segments/wireless/wireless_mcu/debug
    )

    mcux_add_macro(
        TOOLCHAINS armgcc iar
        CC "-DBOARD_DBG_NBU_ENABLE=1"
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
    # Needed to restore UART after power down mode
    mcux_add_macro(HAL_UART_ADAPTER_LOWPOWER=1)
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

include(${SdkRootDirPath}/${board_root}/${board}/project_segments/wireless/prjseg.cmake)

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/gcc/${gcc_wireless_linker_file}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/iar/${iar_wireless_linker_file}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}
    )

endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script_ble)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/gcc/${gcc_wireless_linker_file_ble}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/iar/${iar_wireless_linker_file}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script_ble_loc)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/gcc/${gcc_wireless_linker_file_ble_loc}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/iar/${iar_wireless_linker_file}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.core_linker_script)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/${core_id}/gcc/${gcc_wireless_linker_file}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}/${core_id}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/iar/${core_id}/${iar_wireless_linker_file}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}/${core_id}
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.core_linker_script_ble)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/${core_id}/gcc/${gcc_wireless_linker_file_ble}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}/${core_id}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/wireless_examples/linker/iar/${core_id}/${iar_wireless_linker_file_ble}
        PREPROCESSED TRUE
        PARAMETERS -D${device}${core_id_suffix_name}_SERIES
        INCLUDES examples/_common/project_segments/wireless/${conn_device}/${core_id}
    )
endif()
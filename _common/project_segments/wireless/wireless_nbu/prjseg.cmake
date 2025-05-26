# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.board.wireless.nbu.app.threadx)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        SOURCES
            app_common/main.c
            app_common/app.h
            app_common/README.md
    )
    mcux_add_source(
        TOOLCHAINS iar
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        SOURCES
            app_common/iar/tx_initialize_low_level.s
    )
    mcux_add_source(
        TOOLCHAINS armgcc
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        SOURCES
            app_common/armgcc/tx_initialize_low_level.S
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        INCLUDES
            app_common
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.hw_app)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        SOURCES
            app_common/hardware_init.c
            app_common/app.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        INCLUDES
            app_common
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.board)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        SOURCES
            board.c
            board.h
    )
    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_common/project_segments/wireless/wireless_nbu
        INCLUDES
            .
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
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.wireless.linker_script_ble)
    mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
    )

    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/wireless_examples
        TARGETS debug release
        LINKER linker/gcc/${gcc_wireless_linker_file_ble}
    )

    mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
    )

    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/wireless_examples
        TARGETS debug release
        LINKER linker/iar/${iar_wireless_linker_file}
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
    )
endif()
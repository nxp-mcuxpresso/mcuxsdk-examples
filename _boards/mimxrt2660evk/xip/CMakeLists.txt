if (CONFIG_MCUX_COMPONENT_board.boot_header)
    mcux_add_source(
        SOURCES ${board}_boot_config.c
                ${board}_xspi_nor_config.h
                ${board}_usdhc_config.h
    )

    mcux_add_include(
        INCLUDES .
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        TARGETS debug                        release
                xspi_nor_debug               xspi_nor_release
                psram_debug                  psram_release
                psram_txt_debug              psram_txt_release
                xspi_nor_psram_debug         xspi_nor_psram_release
        CC "-DXIP_BOOT_HEADER_ENABLE=1"
    )
    mcux_add_mdk_configuration(
        TARGETS debug                        release
                xspi_nor_debug               xspi_nor_release
                psram_debug                  psram_release
                psram_txt_debug              psram_txt_release
                xspi_nor_psram_debug         xspi_nor_psram_release
        LD "--predefine=\"-D__xip_boot_header_enable__=1\""
    )

    mcux_add_configuration(
        TARGETS ram_only_debug               ram_only_release
                psram_only_debug             psram_only_release
        CC "-DUSDHC_BOOT_HEADER_ENABLE=0"
    )
    #mcux_add_mdk_configuration(
    #    TARGETS ram_only_debug               ram_only_release
    #            psram_only_debug             psram_only_release
    #    LD "--predefine=\"-D__usdhc_boot_header_enable__=1\""
    #)
    #mcux_add_iar_configuration(
    #    TARGETS ram_only_debug               ram_only_release
    #            psram_only_debug             psram_only_release
    #    LD "--config_def=__usdhc_boot_header_enable__=1"
    #)
    #mcux_add_armgcc_configuration(
    #    TARGETS ram_only_debug               ram_only_release
    #            psram_only_debug             psram_only_release
    #    LD "-Xlinker --defsym=__usdhc_boot_header_enable__=1"
    #)
endif()

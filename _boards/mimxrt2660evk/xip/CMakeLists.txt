if (CONFIG_MCUX_COMPONENT_board.boot_header)
    mcux_add_source(
        SOURCES ${board}_xspi_nor_config.c
                ${board}_xspi_nor_config.h
    )

    mcux_add_include(
        INCLUDES .
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        CC "-DXIP_BOOT_HEADER_ENABLE=1"
    )
    mcux_add_mdk_configuration(
        LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=1\""
    )
endif()
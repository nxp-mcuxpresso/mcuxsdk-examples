mcux_add_cmakelists(${CMAKE_CURRENT_LIST_DIR}/../xip)

if (CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_configuration(
        TARGETS flexspi_nor_debug           flexspi_nor_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
        CC "-DXIP_BOOT_HEADER_ENABLE=0"
    )

    mcux_add_mdk_configuration(
        TARGETS flexspi_nor_debug           flexspi_nor_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
        LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=0\""
    )
endif()

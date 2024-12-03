include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_armgcc_configuration(
        AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
    )

    mcux_add_armgcc_configuration(
        TARGETS sdram_debug                  sdram_release
                sdram_txt_debug              sdram_txt_release
                hyperram_debug               hyperram_release
                hyperram_txt_debug           hyperram_txt_release
                flexspi_nor_sdram_debug      flexspi_nor_sdram_release
                flexspi_nor_hyperram_debug   flexspi_nor_hyperram_release
        AS "-D__STARTUP_INITIALIZE_QADATA"
    )

    mcux_add_armgcc_configuration(
        TARGETS sdram_txt_debug              sdram_txt_release
                hyperram_txt_debug           hyperram_txt_release
                flexspi_nor_debug            flexspi_nor_release
                flexspi_nor_sdram_debug      flexspi_nor_sdram_release
                flexspi_nor_hyperram_debug   flexspi_nor_hyperram_release
        AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
    )

    mcux_add_configuration(
        TARGETS flexspi_nor_debug            flexspi_nor_release
                flexspi_nor_sdram_debug      flexspi_nor_sdram_release
                flexspi_nor_hyperram_debug   flexspi_nor_hyperram_release
        CC "-DXIP_EXTERNAL_FLASH=1"
        CX "-DXIP_EXTERNAL_FLASH=1"
    )

    mcux_add_configuration(
        TARGETS hyperram_debug               hyperram_release
                hyperram_txt_debug           hyperram_txt_release
                flexspi_nor_hyperram_debug   flexspi_nor_hyperram_release
        CC "-DUSE_HYPERRAM=1"
        CX "-DUSE_HYPERRAM=1"
    )

    mcux_add_configuration(
        TARGETS sdram_debug                  sdram_release
                sdram_txt_debug              sdram_txt_release
                flexspi_nor_sdram_debug      flexspi_nor_sdram_release
        CC "-DUSE_SDRAM=1"
        CX "-DUSE_SDRAM=1"
    )
endif()

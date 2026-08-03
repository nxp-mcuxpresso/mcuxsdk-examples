if (CONFIG_MCUX_PRJSEG_module.board.lvgl)
    # Use the non-legacy DBI API (fsl_dbi_spi_dma.h requires MCUX_DBI_LEGACY=0).
    mcux_add_configuration(CC "-DMCUX_DBI_LEGACY=0")

    mcux_add_source(
        SOURCES lvgl_support.h
                lvgl_support.c
    )

    mcux_add_include(
        INCLUDES .
    )

    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/lvgl_examples/lvgl_support/lvgl_support_board.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES ${board_root}/${board}/lvgl_examples/lvgl_support
    )
endif()

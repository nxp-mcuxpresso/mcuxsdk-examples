if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        CC "-DMCUX_META_BUILD"
        CX "-DMCUX_META_BUILD"
    )

    mcux_add_configuration(
        AS "-DMCUXPRESSO_SDK"
        CC "-DMCUXPRESSO_SDK"
        CX "-DMCUXPRESSO_SDK"
    )
endif()

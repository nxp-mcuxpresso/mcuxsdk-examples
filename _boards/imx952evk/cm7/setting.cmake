if (CONFIG_MCUX_PRJSEG_module.board.suite)
   mcux_add_armgcc_configuration(
        AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
    )
endif()

if (CONFIG_MCUX_PRJSEG_module.board.suite)
   mcux_add_armgcc_configuration(
        AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
    )

   mcux_add_mdk_configuration(
        LD "--diag_suppress=6329"
   )
endif()
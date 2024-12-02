if (CONFIG_MCUX_PRJSEG_module.board.suite)
   mcux_add_mdk_configuration(
        LD "--diag_suppress=6329"
   )
endif()
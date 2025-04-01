if (CONFIG_MCUX_PRJSEG_module.board.suite)
   mcux_add_armgcc_configuration(
        CC "-D__STARTUP_INITIALIZE_NONCACHEDATA\
            -D__STARTUP_INITIALIZE_QADATA\
            -D__STARTUP_INITIALIZE_RAMFUNCTION"
    )
endif()

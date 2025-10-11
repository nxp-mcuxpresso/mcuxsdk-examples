if(CONFIG_MCUX_COMPONENT_device.startup)
   mcux_add_armgcc_configuration(
        AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
    )
endif()

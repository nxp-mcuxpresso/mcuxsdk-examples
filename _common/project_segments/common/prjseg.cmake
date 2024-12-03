# console
if(CONFIG_MCUX_PRJSEG_module.board.console)
    mcux_add_configuration(
        CC "-DSDK_DEBUGCONSOLE=1"
        CX "-DSDK_DEBUGCONSOLE=1"
    )
endif()

# console_lite
if(CONFIG_MCUX_PRJSEG_module.board.console_lite)
    mcux_add_configuration(
        CC "-DSDK_DEBUGCONSOLE=1"
        CX "-DSDK_DEBUGCONSOLE=1"
    )
endif()

# boardfile
if(CONFIG_MCUX_PRJSEG_module.board.boardfile)
    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES examples/_boards/${board}/board.h examples/_boards/${board}/board.c)

    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board})
endif()

if(CONFIG_MCUX_PRJSEG_module.board.board_core_file)
    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES examples/_boards/${board}/board_${core_id}.h examples/_boards/${board}/board_${core_id}.c)

    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board})
endif()

if (CONFIG_MCUX_PRJSEG_module.board.use_board_clock)
endif()

# clock
# Use <board>/clock_config
if(CONFIG_MCUX_PRJSEG_module.board.clock)
    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES examples/_boards/${board}/clock_config.h examples/_boards/${board}/clock_config.c)
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board})
endif()
# Use <board>/common/clock/clock_config
if(CONFIG_MCUX_PRJSEG_module.board.clock_board_folder)
    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES examples/_boards/${board}/common/clock/clock_config.h examples/_boards/${board}/common/clock/clock_config.c)
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board}/common/clock)
endif()
# Use <board>/common/clock/<core>/clock_config
if(CONFIG_MCUX_PRJSEG_module.board.clock_board_core_folder)
    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES examples/_boards/${board}/common/clock/${core_id}/clock_config.h examples/_boards/${board}/common/clock/${core_id}/clock_config.c)
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board}/common/clock/${core_id})
endif()

#common_pinmux
# Use <board>/common/pinmux
if(CONFIG_MCUX_PRJSEG_module.board.pinmux_board_folder)
    mcux_add_source(
      BASE_PATH ${SdkRootDirPath}
      SOURCES
        examples/_boards/${board}/common/pin_mux/pin_mux.h
        examples/_boards/${board}/common/pin_mux/pin_mux.c
    )
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board}/common/pin_mux)
endif()

# Use <board>/common/pinmux/<core>
if(CONFIG_MCUX_PRJSEG_module.board.pinmux_board_core_folder)
    mcux_add_source(
      BASE_PATH ${SdkRootDirPath}
      SOURCES
        examples/_boards/${board}/common/pin_mux/${core_id}/pin_mux.h
        examples/_boards/${board}/common/pin_mux/${core_id}/pin_mux.c
    )
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/${board}/common/pin_mux/${core_id})
endif()

# Use <project>/<core>/pinmux
if(CONFIG_MCUX_PRJSEG_module.board.pinmux_project_core_folder)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
      ${project_board_port_path}/${core_id}/pin_mux.c
      ${project_board_port_path}/${core_id}/pin_mux.h
  )
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

# board pinmux without core folder
if(CONFIG_MCUX_PRJSEG_module.board.pinmux_project_folder)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
      ${project_board_port_path}/pin_mux.c
      ${project_board_port_path}/pin_mux.h
  )
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path})
endif()

if(CONFIG_MCUX_PRJSEG_project.hw_app_project_core_folder)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/${core_id}/hardware_init.c
            ${project_board_port_path}/${core_id}/app.h
  )
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

if(CONFIG_MCUX_PRJSEG_project.hw_app_project_folder)
    mcux_add_source(
      BASE_PATH ${SdkRootDirPath}
      SOURCES ${project_board_port_path}/hardware_init.c
              ${project_board_port_path}/app.h
    )
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path})
endif()

# multicore core0 hardware app
if(CONFIG_MCUX_PRJSEG_project.hw_app_core0)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/${core_id}/hardware_init_core0.c
            ${project_board_port_path}/${core_id}/app_core0.h)
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

# multicore core1 hardware app
if(CONFIG_MCUX_PRJSEG_project.hw_app_core1)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/${core_id}/hardware_init_core1.c
            ${project_board_port_path}/${core_id}/app_core1.h)
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

if(CONFIG_MCUX_PRJSEG_project.hw_project_core_folder)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/${core_id}/hardware_init.c
  )
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

if(CONFIG_MCUX_PRJSEG_project.hw_project_folder)
    mcux_add_source(
      BASE_PATH ${SdkRootDirPath}
      SOURCES ${project_board_port_path}/hardware_init.c)
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path})
endif()

# multicore core0 hardware
if(CONFIG_MCUX_PRJSEG_project.hw_core0)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/${core_id}/hardware_init_core0.c)
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

# multicore core1 hardware
if(CONFIG_MCUX_PRJSEG_project.hw_core1)
  mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/${core_id}/hardware_init_core1.c)
  mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path}/${core_id})
endif()

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/project_template/board.c
          ${board_root}/${board}/project_template/board.h
          ${board_root}/${board}/project_template/clock_config.c
          ${board_root}/${board}/project_template/clock_config.h
          ${board_root}/${board}/project_template/peripherals.c
          ${board_root}/${board}/project_template/peripherals.h
)

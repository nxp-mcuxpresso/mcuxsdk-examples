mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/project_template/board.c
          examples/_boards/${board}/project_template/board.h
          examples/_boards/${board}/project_template/clock_config.c
          examples/_boards/${board}/project_template/clock_config.h
          examples/_boards/${board}/project_template/peripherals.c
          examples/_boards/${board}/project_template/peripherals.h
          
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath} 
  INCLUDES examples/_boards/${board}/project_template/
)

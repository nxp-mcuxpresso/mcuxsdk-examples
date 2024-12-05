
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/driver_examples/lin/master/lin_cfg.h
            ${board_root}/kw45b41zevk/driver_examples/lin/master/lin_cfg.c
            ${board_root}/kw45b41zevk/driver_examples/lin/master/./hardware_init.c
            ${board_root}/kw45b41zevk/driver_examples/lin/master/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/driver_examples/lin/master
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/driver_examples/lin/slave/lin_cfg.h
            ${board_root}/kw45b41zevk/driver_examples/lin/slave/lin_cfg.c
            ${board_root}/kw45b41zevk/driver_examples/lin/slave/./hardware_init.c
            ${board_root}/kw45b41zevk/driver_examples/lin/slave/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/driver_examples/lin/slave
)

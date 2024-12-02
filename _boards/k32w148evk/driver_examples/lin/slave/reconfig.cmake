
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/driver_examples/lin/slave/lin_cfg.h
            examples/_boards/kw45b41zevk/driver_examples/lin/slave/lin_cfg.c
            examples/_boards/kw45b41zevk/driver_examples/lin/slave/./hardware_init.c
            examples/_boards/kw45b41zevk/driver_examples/lin/slave/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/driver_examples/lin/slave
)

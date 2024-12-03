
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/demo_apps/hello_world/hardware_init.c
            examples/_boards/evkmimx8ulp/demo_apps/hello_world/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/demo_apps/hello_world
             examples/_boards/evkmimx8ulp
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/iar/MIMX8UD5xxx08_flash.icf
)
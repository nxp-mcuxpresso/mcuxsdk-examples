
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/romapi/common/pin_mux.c
            examples/_boards/${board}/driver_examples/romapi/common/pin_mux.h
            examples/_boards/${board}/driver_examples/romapi/common/hardware_init.c
            examples/_boards/${board}/driver_examples/romapi/common/app.h
)

mcux_add_armgcc_configuration(
    CC "-fomit-frame-pointer"
)
mcux_add_iar_configuration(
    CX "--diag_suppress Pa084"
)

mcux_remove_macro(
    CC "-DMCUXPRESSO_SDK"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x3000\
        -Xlinker --defsym=__heap_size__=0x8000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x8000"
)

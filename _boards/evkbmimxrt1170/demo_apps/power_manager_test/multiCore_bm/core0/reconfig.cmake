
mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)

mcux_add_macro(
    TOOLCHAINS iar mdk
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DCORE1_IMAGE_COPY_TO_RAM\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1\
       -DFSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DCORE1_IMAGE_COPY_TO_RAM\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1\
       -DFSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL=1"
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)



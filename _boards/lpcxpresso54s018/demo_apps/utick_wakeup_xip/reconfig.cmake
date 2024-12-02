
mcux_add_iar_configuration(
    LD "--config_def XIP_IMAGE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    AS "-DXIP_IMAGE"
)
mcux_add_macro(
    AS "-DIMG_BAUDRATE=96000000"
)

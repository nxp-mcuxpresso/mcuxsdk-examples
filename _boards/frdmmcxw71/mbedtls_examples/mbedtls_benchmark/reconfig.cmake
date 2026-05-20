
mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)

mcux_add_macro(
    CC "-DMBEDTLS_NXP_SSSAPI\
       -DMBEDTLS_NXP_ELE200\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM"
)

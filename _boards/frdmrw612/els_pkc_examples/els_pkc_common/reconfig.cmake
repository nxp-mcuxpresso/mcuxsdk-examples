
mcux_add_iar_configuration(
    CX "--diag_suppress Pe177"
)
mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable"
)
mcux_add_mdk_configuration(
    CC "-Wno-tautological-constant-compare"
)

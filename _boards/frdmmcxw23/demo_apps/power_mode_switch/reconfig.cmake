
# Add additional iar configuration
mcux_add_iar_configuration(
    CC "--vla"
    CX "--diag_suppress=Pe144,Pe111,Pe188"
)

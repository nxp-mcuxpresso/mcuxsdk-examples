

# Add additional configuration
mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
    CC "--dlib_config full"
)



# Remove additional configuration
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)

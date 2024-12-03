

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
)

mcux_remove_armgcc_configuration(
    TARGETS debug hyperram_debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug hyperram_debug
    CC "-Og"
    CX "-Og"
)

mcux_add_macro(
    CC "-DUSB_STACK_BM\
       -DSDK_DEBUGCONSOLE=1"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_debug
    LD "--legacyalign\
       --diag_suppress=3912"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    LD "--legacyalign\
       --diag_suppress=3912"
)




mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
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
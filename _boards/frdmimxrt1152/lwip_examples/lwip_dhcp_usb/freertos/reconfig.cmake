



 # Remove targets inherited from arch/soc/board level

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

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
  TARGETS
    "debug"
    "flexspi_nor_debug"
    "flexspi_nor_release"
    "release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "hyperram_debug"
    "hyperram_release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)


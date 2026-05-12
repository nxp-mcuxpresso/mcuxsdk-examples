mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

mcux_add_macro(
    CC "-DLFS_NO_INTRINSICS=1"
)

mcux_add_macro(
    CC "PSA_WANT_ALG_GCM\
        PSA_WANT_ALG_SHA_1"
)

# -------- adjust heap and stack size -----------
# min 8kB needed, for swapping flash sector on pflash 
# with read-modify-write pattern
mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x4000"
)
mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x4000"
)
mcux_add_mdk_configuration(
  LD "--predefine=-D__heap_size__=0x4000"
)
mcux_add_mdk_configuration(
  LD "--predefine=-D__stack_size__=0x4000"
)
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x4000"
)
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x4000"
)
# --------- add suppression flags -----------
mcux_add_iar_configuration(
    CC "--diag_suppress Pe188,Pe177,Pe186"
    CX "--diag_suppress Pe188,Pe177,Pe186"
)
mcux_add_armgcc_configuration(
    CC "-Wno-unused-function"
)
mcux_add_mdk_configuration(
    CC "-Wno-typedef-redefinition"
)

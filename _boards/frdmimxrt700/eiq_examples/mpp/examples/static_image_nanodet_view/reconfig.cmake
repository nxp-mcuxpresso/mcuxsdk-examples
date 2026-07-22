# board+application specific cmake

mcux_add_macro(
    CC "-DRTOS_HEAP_SIZE=2300 \
    -DconfigGENERATE_RUN_TIME_STATS=1"
    CX "DconfigGENERATE_RUN_TIME_STATS=1"
)

#increase __ncache_size__ also increases the size of npu_ncache section accessible to NPU
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__ncache_size__=0x00400000"
)

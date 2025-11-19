# board+application specific cmake

mcux_add_macro(
    CC "-DconfigGENERATE_RUN_TIME_STATS=1 \
    -DRTOS_HEAP_SIZE=1000 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSDK_DEBUGCONSOLE=1"
    CXX "-DconfigGENERATE_RUN_TIME_STATS=1 \
    -DDEBUG_CONSOLE_RX_ENABLE=0 \
    -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING \
    -DSDK_DEBUGCONSOLE=1"
)

#increase __ncache_size__ also increases the size of npu_ncache section accessible to NPU
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__ncache_size__=0x00390000"
)

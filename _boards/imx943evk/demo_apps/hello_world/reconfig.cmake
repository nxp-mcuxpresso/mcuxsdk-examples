# Transparent serial_mux client reuse for the stock hello_world demo.
#
# MUX_REGION_ADDR and MUX_SLOTS_CORES are provided once via the serial_mux
# Kconfig (see prj.conf: CONFIG_MUX_REGION_ADDR / CONFIG_MUX_SLOTS_CORES), so
# they are emitted into mcux_config.h exactly once. Do NOT also inject them
# here with -D or the -Werror "redefined" collision returns.

# Only the cm7 client cores get a MUX_CORE_INDEX; the hub (cm33_core1) is not a
# hello_world client in this scenario (it runs the console_mux hub).
if(${core_id} STREQUAL "cm7_core0")
    mcux_add_configuration(CC "-DMUX_CORE_INDEX=0")
elseif(${core_id} STREQUAL "cm7_core1")
    mcux_add_configuration(CC "-DMUX_CORE_INDEX=1")
endif()

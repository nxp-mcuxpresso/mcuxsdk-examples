# Single-UART all-virtual console mux board configuration.
#
# The shared-memory region base address is provided by the serial_mux
# Kconfig (MUX_REGION_ADDR, a no-prefix hex macro in mcux_config.h,
# default 0x20800000: cm33_core1 system OCRAM on imx943evk).

# Two client cores each own one uplink/downlink slot ring: cm7_core0 -> channel
# 0, cm7_core1 -> channel 1. The hub (cm33_core1) has no slot ring; its own
# MUX_SLOTS_CORES is provided by the serial_mux Kconfig (no-prefix macro in
# mcux_config.h, default 2); the hub console uses channel MUX_SLOTS_CORES,
# which no client core owns.

# One binary is built per core target; the role is chosen at compile time via
# MUX_CORE_INDEX:
#   cm33_core1 = -1  Hub / UART-MUX owner (LPUART8 bridge by default; own
#                    console channel 0, or dedicated LPUART8 console with the
#                    mux bridge on LPUART11 when the dedicated console is on).
#   cm7_core0  =  0  Virtual console client, channel 0.
#   cm7_core1  =  1  Virtual console client, channel 1.
# The client cores route their console through the serial_mux HAL UART
# adapter (components/uart/fsl_adapter_muxuart.c), selected with
#. A component enable alone cannot be
# used here: component options are not emitted into mcux_config.h.
if(${core_id} STREQUAL "cm33_core1")
    mcux_add_configuration(CC "-DMUX_CORE_INDEX=-1")
elseif(${core_id} STREQUAL "cm7_core0")
    mcux_add_configuration(CC "-DMUX_CORE_INDEX=0")
elseif(${core_id} STREQUAL "cm7_core1")
    mcux_add_configuration(CC "-DMUX_CORE_INDEX=1")
endif()

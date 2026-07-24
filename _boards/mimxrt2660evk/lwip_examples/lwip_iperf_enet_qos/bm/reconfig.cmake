include(${CMAKE_CURRENT_LIST_DIR}/../../common/enet_qos/reconfig.cmake)

# Non-blocking console needs the driver transactional UART path on RT2660.
mcux_add_macro(CC "-DHAL_UART_TRANSFER_MODE=1")

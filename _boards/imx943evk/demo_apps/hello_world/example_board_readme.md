Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW1(SYS_PWR) to power on the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Serial mux (optional)
=====================
By default this hello_world runs directly on the physical debug UART (LPUART):
each core opens its own debug console and prints/reads on its own physical port.
No hub or shared memory is involved. This is the standard, recommended setup.

The serial_mux feature is an OPTIONAL alternative in which several cores share a
SINGLE physical UART. Instead of owning a UART, each participating core becomes a
mux "client" whose console traffic is carried over a shared-memory slot region to
a "hub" core. The hub owns the one physical UART and multiplexes every client
console onto it. This is useful when a design has more cores than available
physical debug UARTs.

How the pieces map on IMX943-EVK:
- cm33_core1 : hub  (owns the physical UART, runs the console_mux demo)
- cm7_core0  : client -> logical channel 0
- cm7_core1  : client -> logical channel 1
- Shared-memory slot region base: 0x20800000 (cm33_core1 system OCRAM)

To enable serial_mux for this hello_world:

1.  Edit this example's prj.conf and uncomment the "OPTIONAL: serial_mux" block.
    The single Kconfig option that switches this example onto the serial_mux
    path is:
        CONFIG_MCUX_COMPONENT_component.muxuart_adapter=y
        CONFIG_MCUX_COMPONENT_component.lpuart_adapter=n
    This is MANDATORY. Enabling it routes PRINTF/GETCHAR through the virtual
    debug-console backend and transitively selects the serial_mux core, slots,
    client and transport components for you - so you do NOT need to (and should
    not) list those serial_mux components separately. If you skip this option
    the serial_mux feature does NOT take effect and the example simply builds
    the standard physical-UART path.
    The block also sets the shared-memory geometry:
        CONFIG_MUX_REGION_ADDR=0x20800000
        CONFIG_MUX_SLOTS_CORES=2
    With the virtual backend enabled, cm7_core0 and cm7_core1 build as mux
    clients (see cm7_core0/hardware_init.c and cm7_core1/hardware_init.c).
    Without it (the default), the same files build the standard physical-UART
    path.

2.  A SEPARATE core MUST run the hub. Build and run the console_mux demo
    on cm33_core1:
        examples/_boards/imx943evk/serial_mux_examples/console_mux
    The hub must use the SAME shared-memory region base (0x20800000) and the
    same client-core count (2). It owns the physical UART and prints the
    multiplexed output of all clients.

3.  Flash and run all three cores (hub + both clients). Open a single serial
    terminal (115200 8N1) on the hub's physical UART. You will see the
    "hello world." output from each cm7 client core arriving through the hub.

Note: if you only build/run this hello_world with serial_mux enabled but do NOT
run the console_mux hub on cm33_core1, the client cores will have no hub
to drain their shared-memory slots and no console output will appear.

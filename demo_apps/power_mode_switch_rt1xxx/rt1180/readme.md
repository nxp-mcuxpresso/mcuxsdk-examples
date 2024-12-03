# power_mode_switch_pmic

## Overview
The Power mode switch bm pmic demo application demonstrates the usage of chip's power features with integration of PF5020. 
The CPU mode is power mode of CPU platform. Each CPU platform has its own power mode. They are RUN, WAIT, STOP, SUSPEND.
System sleep mode is the second kind of low power mode besides CPU mode. System sleep mode is related to the state of all
CPU platforms. When all CPU platforms are in low power mode, the system can further shut off some analog modules or put them
into their own STANDBY mode. This chip status is called SYSTEM SLEEP.

This demo prints the power mode menu through the debug console, where the user can set the MCU to a specific power mode.
User can wakeup the core by key interrupt. The purpose of this demo is to show how to switch between different power modes,
and how to configure a wakeup source and wakeup the MCU from low power modes.

In chip PMIC board, DCDC is not used. PF5020's SW1 output(1.1V) is connected to chip's VDD_SOC_IN and SW2 output(1.8V)
is connected to chip's  VDD_1P8_IN. PF5020's 'STANDBY' pin is connected to PMIC_STBY_REQ.


Notes:
1. PMIC is controlled by LPI2C3, to make sure the clock supply for it. Using RC24M as the clock source and make sure the clock gate is enabled under any power mode.



Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin current would be activated unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.
 - Debug pins(e.g SWD_DIO) would consume additional power, had better to disable related pins or disconnect them. 

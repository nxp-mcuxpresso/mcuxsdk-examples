# Getting Started

## Hardware Settings
  Since FRDM-K66 have no USB Vbus control gpio, need user switch the jump setting to control USB Vbus manually,
  otherwise the USB pin detect example can't  detect some low speed device if an adapter cable with a low speed
  mouse connected.

  - The Jumper settings:\n J21 1-2 OFF and J16 1-2 OFF when swith to device mode.
                        \n J21 1-2 ON or J16 1-2 ON after swith to host mode. 

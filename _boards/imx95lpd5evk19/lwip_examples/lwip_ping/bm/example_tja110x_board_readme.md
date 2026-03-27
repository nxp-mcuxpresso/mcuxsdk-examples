Hardware requirements
===================
- Micro USB cable
- IMX95LPD5-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- Connect TJA110x SDBR board under IMX95LPD5-EVK.

Board settings
============
To test iMX95EVK + TJA1103/4 SDBR:
- configure lwip_netc_port.h with:
    - NETC_MII_SPEED kNETC_MiiSpeed100M
    - NETC_PSI0     kNETC_ENETC1PSI0
    - Select kNETC_RmiiMode to use RMII 

*Note: to use RMII reworks are required on IMX95LPD5-EVK and TJA110x SDBR
(see https://github.com/nxp-imx/linux-imx/blob/lf-6.12.y/arch/arm64/boot/dts/freescale/imx95-19x19-evk-tja1104-rmii.dts#L6-L25).


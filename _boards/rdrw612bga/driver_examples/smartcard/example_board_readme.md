Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer
- GSM sim card
- SIM CARD Board

Board settings
============
Remove JP44,JP50
Load Jumper on JP33(2)-HD5 and SIM card VCC/IO is required 3.3V.
Connect R537

SIM CARD Board CONNECTS RW61X Board
Pin Name                Pin Name           Board Location
SIM_VCC                 VCC_SDIO_HOST      HD10-9
SIM_RST                 GPIO16_SD_D3       HD10-3
SIM_CLK                 GPIO15_SD_CLK      HD10-11
SIM_SW1                 GPIO19_SD_D0       HD10-13
SIM_IO                  GPIO17_SD_CMD      HD10-5
SIM_VCC_CNTL            GPIO18_SD_D2       HD10-1
GND                     GND                HD10-7

Note:
Card insert detection is not supported since there is no card insert detection circuit on this board.

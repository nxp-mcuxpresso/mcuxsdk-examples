Supported Wi-Fi/BT/BLE modules
==============================
  - AzureWave FRDM-IW416-AW-AM510
  - Embedded Artists 2LL M.2 Module (EAR00500)


Modules Settings
================
AzureWave FRDM-IW416-AW-AM510 Module:
  - J4  1-2: 1.8V (VIO)
  - J7  2-3: 3.3V (SDIO voltage level - Arduino header)
  - J16 2-3: 3.3V (WL_RST voltage level - Arduino header)
  - connect antenna to J1
Embedded Artists 2LL module datasheet: https://www.embeddedartists.com/wp-content/uploads/2024/12/2LL_M2_Datasheet.pdf
X-FRDM-WiFi-M.2 module is inserted into FRDM-MCXN947T as an adapter for 2LL M.2 Module.
  - J6  1-2: SDIO_WAKE (Wi-Fi Host Sleep Wakeup - X-FRDM-WiFi-M.2)
  - J7  1-2: SDIO_RST  (Wi-Fi Independent Reset OOB - X-FRDM-WiFi-M.2)
  - J8  2-3: 1.8V (X-FRDM-WiFi-M.2)
  - J15 2-3: 1.8V (X-FRDM-WiFi-M.2)
  - J16 1-2: 3.3V (X-FRDM-WiFi-M.2)
  - J17 2-3: 1.8V (X-FRDM-WiFi-M.2)
  - J18 1-2: 3.3V (X-FRDM-WiFi-M.2)
  - J19 2-3: 1.8V (X-FRDM-WiFi-M.2)
  - J24 1-2: 3.3V (X-FRDM-WiFi-M.2)
  - J25 1-2: 3.3V (X-FRDM-WiFi-M.2)
  - J26 1-2: WL_RST (X-FRDM-WiFi-M.2)

Board settings
==============
AzureWave FRDM-IW416-AW-AM510 Module:
  - Connect additional USB: J11.
Embedded Artists 2LL M.2 Module:
  - None

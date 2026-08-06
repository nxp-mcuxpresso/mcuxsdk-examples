# coex_wifi_handsfree_ag - MIMXRT1170-EVKB

## Overview
Coexistence of the Bluetooth HFP Audio Gateway (AG) role (edgefast_open) and Wi-Fi on the
MIMXRT1170-EVKB with an IW612 or IW416 M.2 module. The coex controller downloads the NB (BT)
firmware over UART, then brings up WLAN over SDIO. BT and Wi-Fi then run concurrently: the AG
discovers/connects an HF unit and drives calls with SCO voice audio (WM8962 codec) while
Wi-Fi runs scan / ping / iperf.

## Supported modules
- IW612 (Murata 2EL M.2) - default
- IW416 (Murata 1XK M.2)

The Wi-Fi/BT module is selected in `_boards/evkbmimxrt1170/coex_examples/coex_wifi_handsfree_ag/cm7/prj.conf`.

For IW612 (Embedded Artists / Murata 2EL M.2 - default):
```
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW61X=y
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_2el_m2=y
```
For IW416 (Murata 1XK M.2):
```
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.IW416=y
CONFIG_MCUX_COMPONENT_component.wifi_bt_module.board_murata_1xk_m2=y
```

## Hardware requirements
- MIMXRT1170-EVKB board
- IW612 or IW416 M.2 module (direct M.2 connection)
- Micro USB cable
- Personal computer
- A headset (or speaker + microphone) connected to the on-board WM8962 audio jack for SCO voice
- A second HFP Hands-Free unit: a BT headset, or another board running `coex_wifi_handsfree`

## Board settings
- Insert the M.2 module into the M.2 slot.
- Connect a micro USB cable between the PC and the MCU-Link debug USB port (J7).
- Connect the headset/speaker+mic to the WM8962 audio jack.

Jumper settings for RT1170-EVKB (external 5V supply for the M.2 module):
- remove  J38 5-6
- connect J38 1-2
- connect J43 with external power (controlled by SW5)

**NOTE:**
1. The LITTLEFS flash region is used for BT bonding storage. Erase all flash sectors before
   downloading the example to ensure a clean storage region.
2. After flashing to QSPI flash and booting from it, reset the board (SW7, or power-cycle) to
   run the application.

## Build
Prerequisites: CMake (>=3.24), Ninja (>=1.12), ARM GCC Toolchain (**only ARM GCC is supported**), Python3 (>=3.6).

```bash
cd <sdk root>
# flexspi_nor_release (recommended for coex):
west build -b evkbmimxrt1170 examples/coex_examples/coex_wifi_handsfree_ag \
  -p always -d build/coex_wifi_handsfree_ag --config flexspi_nor_release \
  --toolchain armgcc -- -Dcore_id=cm7
```
Output: `build/coex_wifi_handsfree_ag/coex_wifi_handsfree_ag_cm7.elf` / `.bin`.

## Flash
```
# J-Link: write the coex app image to QSPI flash
J-Link> loadbin <path>/coex_wifi_handsfree_ag_cm7.bin, 0x30000400
```

## Prepare the demo
1. Connect the micro USB cable between the PC and the MCU-Link USB port (J7).
2. Open a serial terminal: 115200 baud, 8 data bits, no parity, 1 stop bit, no flow control.
3. Download the program to the target board.
4. Reset the SoC to run the application.

## Running the demo
On boot the coex controller downloads the NB (BT) firmware over UART and initializes WLAN over
SDIO, then the `@Coex>` shell appears. Both `bt` (HFP-AG) and `wifi` (Wi-Fi CLI) commands are
available at the single prompt. Wi-Fi commands become available after WLAN initialization.

### bt command (HFP Audio Gateway control)
```
@Coex> bt
  discover                 find nearby BT devices
  connect <n>              connect to the n-th discovered device (from 1)
  select_ag <0|1>          select which AG connection subsequent commands act on
  openaudio / closeaudio   open / close an SCO audio connection without a call
  sincall                  start an incoming call on the selected AG connection
  stwcincall               start a second (call-waiting) incoming call
  aincall [index]          accept the call (optional call index)
  eincall [index]          end / reject the call (optional call index)
  select_codec <n>         select codec for negotiation (1 = CVSD, 2 = mSBC)
  set_mic_volume <v>       set microphone volume (1..15)
  set_speaker_volume <v>   set speaker volume (1..15)
  set_hf_ind <1|2> <enable|disable>   HF indicator (1 = enh driver safety, 2 = battery)
  disconnect <index>       disconnect the ACL connection at the given index
  delete                   delete all bonded devices (disconnect first)
```

### wifi command (Wi-Fi CLI dispatch)
Any Wi-Fi CLI command is issued with the `wifi` prefix. Use `wifi help` for the full list.
```
@Coex> wifi wlan-version
@Coex> wifi wlan-scan
@Coex> wifi wlan-add <profile> ssid <SSID> wpa2 psk <password>
@Coex> wifi wlan-connect <profile>
@Coex> wifi wlan-address
@Coex> wifi ping <ip>
@Coex> wifi iperf -c <ip> -u -B <local-ip> -b 200 -t 20 -p 5001
```

### Example: HFP call + Wi-Fi coexistence (AG side)
The AG initiates the connection to the HF unit and drives the call:
```
@Coex> bt discover              # list nearby devices; note the HF index (e.g. "hfp_hf")
@Coex> bt connect 1             # connect to the HF (wait for HFP service level connection)
@Coex> bt select_ag 0
@Coex> bt sincall               # ring the HF; accept it on the HF side (bt aincall)
@Coex> bt set_speaker_volume 12
@Coex> bt set_mic_volume 12
# with the call active, run Wi-Fi traffic simultaneously:
@Coex> wifi wlan-add sta ssid MyAP wpa2 psk MyPassword
@Coex> wifi wlan-connect sta
@Coex> wifi ping 192.168.0.1
@Coex> bt eincall               # end the call
```
To test the SCO audio path without a call: `bt openaudio` then `bt closeaudio`.

**NOTE:** a short codec power-on pop noise at the start/end of SCO audio streaming is expected
and cannot be eliminated (WM8962 codec characteristic).

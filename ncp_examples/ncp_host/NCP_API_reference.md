# Main Page

## Introduction

NXP NCP host sample application provides reference code of communication
between NCP host and NCP device over NCP TLV protocol. NCP host could
connect to redfinch NCP device through multiple HW interfaces.
Developers can refer to NCP APIs for Wi-Fi, Bluetooth LE and 15.4 to
implement their own sample application which runs on their host
platform.

Note:
Default NCP host MCU platform: RT1060 EVKB.
NCP device supported two kinds redfinch boards:
RD-RW612-BGA: UART/SPI/USB/SDIO,
FRDM-RW612: UART/SPI/USB.

### Abbreviations and acronyms

<table>
<tbody>
<tr class="odd">
<td>Abbreviations</td>
<td>Description</td>
</tr>
<tr class="even">
<td>ACS</td>
<td>auto channel selection</td>
</tr>
<tr class="odd">
<td>AD</td>
<td>advertising data</td>
</tr>
<tr class="even">
<td>AES</td>
<td>advanced encryption standard</td>
</tr>
<tr class="odd">
<td>AP</td>
<td>access point</td>
</tr>
<tr class="even">
<td>ARP</td>
<td>address resolution protocol</td>
</tr>
<tr class="odd">
<td>BAS</td>
<td>battery service</td>
</tr>
<tr class="even">
<td>BIG</td>
<td>broadcast isochronous group</td>
</tr>
<tr class="odd">
<td>BSS</td>
<td>basic service set</td>
</tr>
<tr class="even">
<td>BSSID</td>
<td>basic service set ID</td>
</tr>
<tr class="odd">
<td>CCC</td>
<td>client characteristic configuration</td>
</tr>
<tr class="even">
<td>CCK</td>
<td>complementary code keying</td>
</tr>
<tr class="odd">
<td>CSI</td>
<td>channel state information</td>
</tr>
<tr class="even">
<td>CSIS</td>
<td>coordinated set identification service</td>
</tr>
<tr class="odd">
<td>CW</td>
<td>continuous wave</td>
</tr>
<tr class="even">
<td>DHCP</td>
<td>dynamic host configuration protocol</td>
</tr>
<tr class="odd">
<td>DPP</td>
<td>device provisioning protocol</td>
</tr>
<tr class="even">
<td>DTIM</td>
<td>delivery traffic indication map</td>
</tr>
<tr class="odd">
<td>DNS</td>
<td>domain name system</td>
</tr>
<tr class="even">
<td>EAP</td>
<td>extensible authentication protocol</td>
</tr>
<tr class="odd">
<td>EATT</td>
<td>enhanced attribute protocol</td>
</tr>
<tr class="even">
<td>EAP-TLS</td>
<td>extensible authentication protocol transport layer security</td>
</tr>
<tr class="odd">
<td>EAP-TTLS</td>
<td>extensible authentication protocol tunneled transport layer security</td>
</tr>
<tr class="even">
<td>EIR</td>
<td>extended inquiry response</td>
</tr>
<tr class="odd">
<td>EU</td>
<td>encrypt unit</td>
</tr>
<tr class="even">
<td>FCS</td>
<td>frame check sequence</td>
</tr>
<tr class="odd">
<td>FT</td>
<td>fast BSS transition</td>
</tr>
<tr class="even">
<td>GAP</td>
<td>generic access profile</td>
</tr>
<tr class="odd">
<td>GATT</td>
<td>generic attribute profile</td>
</tr>
<tr class="even">
<td>GI</td>
<td>guard interval</td>
</tr>
<tr class="odd">
<td>HE</td>
<td>802.11ax high efficiency</td>
</tr>
<tr class="even">
<td>HRC</td>
<td>health rate client</td>
</tr>
<tr class="odd">
<td>HT</td>
<td>802.11n high throughput</td>
</tr>
<tr class="even">
<td>HTC</td>
<td>health thermometer client</td>
</tr>
<tr class="odd">
<td>HTS</td>
<td>health thermometer service</td>
</tr>
<tr class="even">
<td>ICMP</td>
<td>internet control message protocol</td>
</tr>
<tr class="odd">
<td>L2CAP</td>
<td>logical link control and adaptation layer protocol</td>
</tr>
<tr class="even">
<td>MBO</td>
<td>multi band operation</td>
</tr>
<tr class="odd">
<td>MDNS</td>
<td>multicast DNS</td>
</tr>
<tr class="even">
<td>MEF</td>
<td>memory efficient filtering</td>
</tr>
<tr class="odd">
<td>MFPC</td>
<td>management frame protection capable</td>
</tr>
<tr class="even">
<td>MFPR</td>
<td>management frame protection required</td>
</tr>
<tr class="odd">
<td>MITM</td>
<td>man in the middle</td>
</tr>
<tr class="even">
<td>MTU</td>
<td>max transmission unit</td>
</tr>
<tr class="odd">
<td>NAK</td>
<td>net access key</td>
</tr>
<tr class="even">
<td>NF</td>
<td>noise floor</td>
</tr>
<tr class="odd">
<td>OCE</td>
<td>optimized connectivity experience</td>
</tr>
<tr class="even">
<td>OFDM</td>
<td>orthogonal frequency division multiplexing</td>
</tr>
<tr class="odd">
<td>OMI</td>
<td>operating mode indication</td>
</tr>
<tr class="even">
<td>OWE</td>
<td>opportunistic wireless encryption</td>
</tr>
<tr class="odd">
<td>OOB</td>
<td>out of band</td>
</tr>
<tr class="even">
<td>OT</td>
<td>openthread</td>
</tr>
<tr class="odd">
<td>PBC</td>
<td>push button configuration</td>
</tr>
<tr class="even">
<td>PHY</td>
<td>physical</td>
</tr>
<tr class="odd">
<td>PIN</td>
<td>personal identification number</td>
</tr>
<tr class="even">
<td>PMF</td>
<td>protected management frame</td>
</tr>
<tr class="odd">
<td>PPE</td>
<td>physical layer (PHY) packet extension</td>
</tr>
<tr class="even">
<td>PS</td>
<td>power save</td>
</tr>
<tr class="odd">
<td>PSK</td>
<td>pre-shared key</td>
</tr>
<tr class="even">
<td>PSM</td>
<td>protocol/service multiplexer</td>
</tr>
<tr class="odd">
<td>PTR</td>
<td>pointer records</td>
</tr>
<tr class="even">
<td>PWE</td>
<td>Password Element</td>
</tr>
<tr class="odd">
<td>QoS</td>
<td>quality of service</td>
</tr>
<tr class="even">
<td>RSI</td>
<td>Resolvable Set Identifier</td>
</tr>
<tr class="odd">
<td>RSSI</td>
<td>received signal strength indicator</td>
</tr>
<tr class="even">
<td>SAD</td>
<td>software antenna diversity</td>
</tr>
<tr class="odd">
<td>SAE</td>
<td>simultaneous authentication of equals</td>
</tr>
<tr class="even">
<td>SIFS</td>
<td>short interframe space</td>
</tr>
<tr class="odd">
<td>SNR</td>
<td>signal noise ratio</td>
</tr>
<tr class="even">
<td>SRV</td>
<td>service records</td>
</tr>
<tr class="odd">
<td>SSID</td>
<td>service set ID</td>
</tr>
<tr class="even">
<td>STBC</td>
<td>space time block code</td>
</tr>
<tr class="odd">
<td>TBTT</td>
<td>target beacon transmission time</td>
</tr>
<tr class="even">
<td>TWT</td>
<td>target wake time</td>
</tr>
<tr class="odd">
<td>UAPSD</td>
<td>unscheduled automatic power save delivery</td>
</tr>
<tr class="even">
<td>UUID</td>
<td>universally unique identifier</td>
</tr>
<tr class="odd">
<td>URI</td>
<td>uniform resource identifier</td>
</tr>
<tr class="even">
<td>VHT</td>
<td>802.11ac very high throughput</td>
</tr>
<tr class="odd">
<td>WEP</td>
<td>wired equivalent privacy</td>
</tr>
<tr class="even">
<td>WLCMGR</td>
<td>Wi-Fi command manager</td>
</tr>
<tr class="odd">
<td>WPA</td>
<td>Wi-Fi protected access</td>
</tr>
<tr class="even">
<td>WPA2</td>
<td>Wi-Fi protected access 2</td>
</tr>
<tr class="odd">
<td>WPA3</td>
<td>Wi-Fi protected access 3</td>
</tr>
<tr class="even">
<td>WPS</td>
<td>Wi-Fi protected setup</td>
</tr>
<tr class="odd">
<td>WMM</td>
<td>Wi-Fi multimedia</td>
</tr>
<tr class="even">
<td>WNM</td>
<td>wireless network management</td>
</tr>
<tr class="odd">
<td>WOWLAN</td>
<td>wake on wireless LAN</td>
</tr>
</tbody>
</table>

# File Index

## File List

Here is a list of all documented files with brief descriptions:

**bas.h (Battery Service Profile definitions )**

**hr.h (Health Rate Profile definitions )**

**ht.h (Health Thermometer Profile definitions )**

**ncp\_cmd\_ble.h (NCP Bluetooth LE command and structure definitions )**

**ncp\_cmd\_wifi.h (NCP Wi-Fi command/response definitions )**

**ncp\_host\_command\_ble.h (NCP Bluetooth LE host command interfaces )**

**ncp\_host\_command\_wifi.h (NCP host command interfaces )**

**otopcode.h (This file provides interface to get Open Thread commands corresponding opcode )**

**service.h (Bluetooth service definitions )**

# Data Structure Documentation

## \_ACSBand\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint16\_t acs\_band

### Detailed Description

This structure is used for ACS (auto channel selection) band
configuration.

### Field Documentation

#### TypeHeader\_t \_ACSBand\_ParamSet\_t::header

> Header type and size information.

#### uint16\_t \_ACSBand\_ParamSet\_t::acs\_band

> Band configuration for ACS. Valid when set channel to zero.
> 
> 0: 2GHz,
> 
> 1: 5GHz.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_BSSID\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - char bssid \[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

### Detailed Description

This structure is used for BSSID configuration.

### Field Documentation

#### TypeHeader\_t \_BSSID\_ParamSet\_t::header

> Header type and size information.

#### char \_BSSID\_ParamSet\_t::bssid\[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

> BSSID (basic service set identifier).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_BSSRole\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t role

### Detailed Description

This structure is used for BSS role configuration.

### Field Documentation

#### TypeHeader\_t \_BSSRole\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_BSSRole\_ParamSet\_t::role

> Network BSS role,
> 
> 0: STA,
> 
> 1: UAP.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_CAPA\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t capa

### Detailed Description

This structure is used for Wi-Fi capabilities configuration.

### Field Documentation

#### TypeHeader\_t \_CAPA\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_CAPA\_ParamSet\_t::capa

> Wi-Fi capabilities bitmap, such as 802.11n, 802.11ac, 802.11ax and
> legacy.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_Channel\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t channel

### Detailed Description

This structure is used for channel configuration.

### Field Documentation

#### TypeHeader\_t \_Channel\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_Channel\_ParamSet\_t::channel

> Network channel

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_DTIM\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t dtim\_period

### Detailed Description

This structure is used for DTIM (delivery traffic indication message)
period configuration.

### Field Documentation

#### TypeHeader\_t \_DTIM\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_DTIM\_ParamSet\_t::dtim\_period

> DTIM (delivery traffic indication message) period.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_EAP\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - char anonymous\_identity \[IDENTITY\_MAX\_LENGTH\]

  - char client\_key\_passwd \[PASSWORD\_MAX\_LENGTH\]

### Detailed Description

This structure is used for EAP (extensible authentication protocol)
information configuration.

### Field Documentation

#### TypeHeader\_t \_EAP\_ParamSet\_t::header

> Header type and size information.

#### char \_EAP\_ParamSet\_t::anonymous\_identity\[IDENTITY\_MAX\_LENGTH\]

> Anonymous identity string for EAP

#### char \_EAP\_ParamSet\_t::client\_key\_passwd\[PASSWORD\_MAX\_LENGTH\]

> Client key password string

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_HE\_CAP\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t ext\_id

  - uint8\_t he\_mac\_cap \[6\]

  - uint8\_t he\_phy\_cap \[11\]

  - uint8\_t he\_txrx\_mcs\_support \[4\]

  - uint8\_t val \[28\]

### Detailed Description

This structure is used for HE (802.11ax high efficient) capability
configuration.

### Field Documentation

#### TypeHeader\_t \_HE\_CAP\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_HE\_CAP\_ParamSet\_t::ext\_id

> Element ID extension.

#### uint8\_t \_HE\_CAP\_ParamSet\_t::he\_mac\_cap\[6\]

> HE MAC capabilities information. The bit values of this bit field are
> defined in the 802.11ax core specification.

#### uint8\_t \_HE\_CAP\_ParamSet\_t::he\_phy\_cap\[11\]

> HE physical capabilities information.The bit values of this bit field
> are defined in the 802.11ax core specification.

#### uint8\_t \_HE\_CAP\_ParamSet\_t::he\_txrx\_mcs\_support\[4\]

> HE TX/RX MCS support for 80MHz. The bit values of this bit field are
> defined in the 802.11ax core specification.

#### uint8\_t \_HE\_CAP\_ParamSet\_t::val\[28\]

> Including he\_txrx\_mcs\_support for 160 and 80+80 MHz, and PPE
> Thresholds. The bit values of this bit field are defined in the
> 802.11ax core specification.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_IEEE\_BTWT\_ParamSet\_t Struct Reference

### Data Fields

  - uint16\_t request\_type

  - uint16\_t target\_wake\_time

  - uint8\_t nominal\_min\_wake\_duration

  - uint16\_t wake\_interval\_mantissa

  - uint16\_t twt\_info

### Detailed Description

This structure is used for IEEE BTWT (broadcast target wake time)
configuration.

### Field Documentation

#### uint16\_t \_IEEE\_BTWT\_ParamSet\_t::request\_type

> Request type, bit0: request,
> 
> bit1-bit3: setup\_cmd,
> 
> bit4: trigger,
> 
> bit5: last broadcast parameter set,
> 
> bit6: flow type,
> 
> bit7-bit9: btwt\_recommendation,
> 
> bit10-bit14: wake interval exponent,
> 
> bit15: reserved.

#### uint16\_t \_IEEE\_BTWT\_ParamSet\_t::target\_wake\_time

> Wake up time agreed by the device and host.

#### uint8\_t \_IEEE\_BTWT\_ParamSet\_t::nominal\_min\_wake\_duration

> Nominal minimum TWT wake duration.

#### uint16\_t \_IEEE\_BTWT\_ParamSet\_t::wake\_interval\_mantissa

> TWT wake interval mantissa.

#### uint16\_t \_IEEE\_BTWT\_ParamSet\_t::twt\_info

> Bit0-bit2: reserved
> 
> bit3-bit7: btwt\_id
> 
> bit8-bit15: btwt\_persistence.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_IP\_ADDR\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t addr\_type

  - union {

  -   uint32\_t **ip\_v4**

  -   uint32\_t **ip\_v6** \[4\]

  - } ip

### Detailed Description

This structure is used for IP address configuration.

### Field Documentation

#### TypeHeader\_t \_IP\_ADDR\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_IP\_ADDR\_ParamSet\_t::addr\_type

> IP address type.
> 
> 1: ADDR\_TYPE\_DHCP, use DHCP (dynamic host configuration protocol) to
> obtain the IP address.
> 
> 0: ADDR\_TYPE\_STATIC, use a static IP address.

#### union { ... } \_IP\_ADDR\_ParamSet\_t::ip

> IPv4/IPv6 address.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_IP\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t is\_autoip

  - uint32\_t address

  - uint32\_t gateway

  - uint32\_t netmask

  - uint32\_t dns1

  - uint32\_t dns2

### Detailed Description

This structure is used for network IP configuration.

### Field Documentation

#### TypeHeader\_t \_IP\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_IP\_ParamSet\_t::is\_autoip

> Auto IP flag,
> 
> 0: not auto IP,
> 
> 1: auto IP.

#### uint32\_t \_IP\_ParamSet\_t::address

> Network IP address.

#### uint32\_t \_IP\_ParamSet\_t::gateway

> Gateway.

#### uint32\_t \_IP\_ParamSet\_t::netmask

> Netmask.

#### uint32\_t \_IP\_ParamSet\_t::dns1

> System's primary DNS (domain name system) server.

#### uint32\_t \_IP\_ParamSet\_t::dns2

> System's secondary DNS server.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_MBO\_NONPREFER\_CH\_SUPP Struct Reference

### Data Fields

  - char mbo\_nonprefer\_ch\_params \[32\]

### Detailed Description

This structure is used for MBO non prefer channel configuration.

### Field Documentation

#### char \_MBO\_NONPREFER\_CH\_SUPP::mbo\_nonprefer\_ch\_params\[32\]

> MBO non prefer channel parameters.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_ADD\_SERVICE\_RP Struct Reference

### Data Fields

  - uint16\_t attr\_handle

### Detailed Description

This structure contains the command response of the Bluetooth LE add
service command.

### Field Documentation

#### uint16\_t \_NCP\_ADD\_SERVICE\_RP::attr\_handle

> service attribute handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_ATTR\_VALUE\_CHANGED\_EV Struct Reference

### Data Fields

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[MAX\_ATTRIBUTE\_VALUE\_LEN\]

### Detailed Description

This structure contains the value of the Bluetooth LE GATT attribute
value changed event which notifies the changed attributes to the client.

### Field Documentation

#### uint16\_t \_NCP\_ATTR\_VALUE\_CHANGED\_EV::handle

> attribute handle

#### uint16\_t \_NCP\_ATTR\_VALUE\_CHANGED\_EV::data\_length

> attribute data length

#### uint8\_t \_NCP\_ATTR\_VALUE\_CHANGED\_EV::data\[MAX\_ATTRIBUTE\_VALUE\_LEN\]

> attribute data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_ncp\_ble\_low\_power\_mode\_cfg Struct Reference

### Data Fields

  - uint8\_t power\_mode

  - uint16\_t timeout

### Detailed Description

This structure contains the configuration values of the narrowband
controller low power mode.

### Field Documentation

#### uint8\_t \_ncp\_ble\_low\_power\_mode\_cfg::power\_mode

> Set narrowband controller power mode.
> 
> auto sleep disable : 0x02
> 
> auto sleep enable : 0x03

#### uint16\_t \_ncp\_ble\_low\_power\_mode\_cfg::timeout

> sleep timeout value (the timebase is us)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_ncp\_ble\_set\_bd\_address\_cfg Struct Reference

### Data Fields

  - uint8\_t paramater\_id

  - uint8\_t bd\_addr\_len

  - uint8\_t bd\_address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set device address command.

### Field Documentation

#### uint8\_t \_ncp\_ble\_set\_bd\_address\_cfg::paramater\_id

> Vendor command parameter id.
> 
> The id of Bluetooth LE Vendor set board address is 0xFE

#### uint8\_t \_ncp\_ble\_set\_bd\_address\_cfg::bd\_addr\_len

> Bluetooth LE Device address length
> 
> The length of Bluetooth LE Device address is 0x06

#### uint8\_t \_ncp\_ble\_set\_bd\_address\_cfg::bd\_address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE Device address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CCC\_CFG\_CHANGED\_EV Struct Reference

### Data Fields

  - uint16\_t ccc\_value

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the value of the Bluetooth LE client
characteristic configuration changed event which indicates whether the
characteristic notification or indication is enabled or disabled to the
client.

### Field Documentation

#### uint16\_t \_NCP\_CCC\_CFG\_CHANGED\_EV::ccc\_value

> client characteristic configuration changed value

#### uint8\_t \_NCP\_CCC\_CFG\_CHANGED\_EV::uuid\_length

> UUID length

#### uint8\_t \_NCP\_CCC\_CFG\_CHANGED\_EV::uuid\[SERVER\_MAX\_UUID\_LEN\]

> UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CFG\_SUBSCRIBE\_CMD Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t enable

  - uint16\_t ccc\_handle

### Detailed Description

This structure is used for NCP Bluetooth LE subscribe service.

### Field Documentation

#### uint8\_t \_NCP\_CFG\_SUBSCRIBE\_CMD::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CFG\_SUBSCRIBE\_CMD::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t \_NCP\_CFG\_SUBSCRIBE\_CMD::enable

> subscribe value
> 
> 0: disallow to subscribe
> 
> 1: allow to subscribe

#### uint16\_t \_NCP\_CFG\_SUBSCRIBE\_CMD::ccc\_handle

> ccc (client characteristic configuration) handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_11AX\_CFG Struct Reference

### Data Fields

  - uint8\_t band

  - HE\_CAP\_ParamSet\_t he\_cap\_tlv

### Detailed Description

This structure is used for band type and HE (802.11ax high efficient)
capability configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_11AX\_CFG::band

> Band,
> 
> 0: 2.4GHz,
> 
> 2: 5GHz,
> 
> 3: 2.4GHz and 5GHz.

#### HE\_CAP\_ParamSet\_t \_NCP\_CMD\_11AX\_CFG::he\_cap\_tlv

> HE (802.11ax high efficient) capability.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_11D\_ENABLE Struct Reference

### Data Fields

  - uint32\_t role

  - uint32\_t state

### Detailed Description

This structure is used for 802.11d flag configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_11D\_ENABLE::role

> Role type,
> 
> 0: STA,
> 
> 1: UAP.

#### uint32\_t \_NCP\_CMD\_11D\_ENABLE::state

> State type,
> 
> 0: disable 802.11d,
> 
> 1: enable 802.11d.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_11K\_CFG Struct Reference

### Data Fields

  - int enable

### Detailed Description

This structure is used for enable/disable 802.11k flag configuration.

### Field Documentation

#### int \_NCP\_CMD\_11K\_CFG::enable

> Enable/disable 802.11k flag,
> 
> 0: disable,
> 
> 1: enable.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_ANTENNA\_CFG Struct Reference

### Data Fields

  - uint8\_t action

  - uint32\_t antenna\_mode

  - uint16\_t evaluate\_time

  - uint8\_t evaluate\_mode

  - uint16\_t current\_antenna

### Detailed Description

This structure is used for antenna configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_ANTENNA\_CFG::action

> Action type,
> 
> 0: get,
> 
> 1: set

#### uint32\_t \_NCP\_CMD\_ANTENNA\_CFG::antenna\_mode

> Antenna mode,
> 
> 0: 1ANT,
> 
> 1: 2ANT,
> 
> 15: ANT diversity.

#### uint16\_t \_NCP\_CMD\_ANTENNA\_CFG::evaluate\_time

> SAD (software antenna diversity) evaluate time interval (unit:
> milliseconds), default value is 6000ms.

#### uint8\_t \_NCP\_CMD\_ANTENNA\_CFG::evaluate\_mode

> Evaluate mode is used to specify two out of three antennas for SAD,
> with valid values of 0, 1, 2 and 255.
> 
> 0: ant 1 and ant 2,
> 
> 1: ant 2 and ant 3,
> 
> 2: ant 1 and ant 3,
> 
> 255: indicates that no antenna is specified.

#### uint16\_t \_NCP\_CMD\_ANTENNA\_CFG::current\_antenna

> Current antenna configuration.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_BTWT\_CFG Struct Reference

### Data Fields

  - uint16\_t action

  - uint16\_t sub\_id

  - uint8\_t nominal\_wake

  - uint8\_t max\_sta\_support

  - uint16\_t twt\_mantissa

  - uint16\_t twt\_offset

  - uint8\_t twt\_exponent

  - uint8\_t sp\_gap

### Detailed Description

This structure is used for BTWT configuration. The AP manages BTWT
working mechanism: the AP announces the TWT time period of the current
round in each beacon frame. In some cases, the AP also announces TWT
time period in other management frames. For example association frames,
reassociation frames, and/or probe response frames. The endpoint must
apply to the AP to add a group before the endpoint can run broadcast
TWT. The STA adds the group and follows the latest TWT time period. When
TWT time period is reached, the STA wakes up and starts exchanging
information.

### Field Documentation

#### uint16\_t \_NCP\_CMD\_BTWT\_CFG::action

> Action, only support 1: set.

#### uint16\_t \_NCP\_CMD\_BTWT\_CFG::sub\_id

> Subcommand ID, the subcommand ID of BTWT is 0x125.

#### uint8\_t \_NCP\_CMD\_BTWT\_CFG::nominal\_wake

> Nominal minimum TWT wake duration.

#### uint8\_t \_NCP\_CMD\_BTWT\_CFG::max\_sta\_support

> Maximum number of STAs supported.

#### uint16\_t \_NCP\_CMD\_BTWT\_CFG::twt\_mantissa

> TWT wake interval mantissa, range: \[0 - (2^16-1)\].

#### uint16\_t \_NCP\_CMD\_BTWT\_CFG::twt\_offset

> TWT wake interval offset.

#### uint8\_t \_NCP\_CMD\_BTWT\_CFG::twt\_exponent

> TWT wake interval exponent.

#### uint8\_t \_NCP\_CMD\_BTWT\_CFG::sp\_gap

> Service period gap.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_CLIENT\_CNT Struct Reference

### Data Fields

  - uint16\_t max\_sta\_count

  - uint8\_t set\_status

  - uint8\_t support\_count

### Detailed Description

This structure is used for maximum number of STA configuration for UAP.

### Field Documentation

#### uint16\_t \_NCP\_CMD\_CLIENT\_CNT::max\_sta\_count

> Maximum number of STAs set in the command.

#### uint8\_t \_NCP\_CMD\_CLIENT\_CNT::set\_status

> Set status.
> 
> 0: success,
> 
> 1: fail.

#### uint8\_t \_NCP\_CMD\_CLIENT\_CNT::support\_count

> Maximum number of STAs supported by the device side.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_CONN\_PARA\_UPDATE Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t addr \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t interval\_min

  - uint16\_t interval\_max

  - uint16\_t latency

  - uint16\_t timeout

### Detailed Description

This structure is used for NCP Bluetooth LE update connection
parameters.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE::type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE::addr\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE::interval\_min

> connection minimal interval
> 
> Minimum value for the connection interval. Must be less than or equal
> to maximum connection interval

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE::interval\_max

> connection maximum interval
> 
> Maximum value for the connection interval. Must be greater than or
> equal to minimum connection interval

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE::latency

> connection latency
> 
> Maximum peripheral latency for the connection in number of subrated
> connection events

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE::timeout

> connection timeout
> 
> Supervision timeout for the Bluetooth LE link

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_CONN\_PARA\_UPDATE\_EV Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t addr \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t interval

  - uint16\_t latency

  - uint16\_t timeout

### Detailed Description

This structure contains the value of the Bluetooth LE connection update
complete event which indicate the connection update procedure has
completed.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE\_EV::type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE\_EV::addr\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE\_EV::interval

> connection interval

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE\_EV::latency

> connection latency
> 
> Maximum peripheral latency for the connection in number of subrated
> connection events

#### uint16\_t \_NCP\_CMD\_CONN\_PARA\_UPDATE\_EV::timeout

> connection timeout
> 
> Supervision timeout for the Bluetooth LE link

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_CONNECT Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t addr \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for NCP Bluetooth LE create connection.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_CONNECT::type

> remote address type

#### uint8\_t \_NCP\_CMD\_CONNECT::addr\[NCP\_BLE\_ADDR\_LENGTH\]

> remote address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_CONNECT\_STAT Struct Reference

### Data Fields

  - uint8\_t ps\_mode

  - uint8\_t uap\_conn\_stat

  - uint8\_t sta\_conn\_stat

### Detailed Description

This structure is used for Wi-Fi connection status configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_CONNECT\_STAT::ps\_mode

> PS (power save) mode.
> 
> 0: active mode,
> 
> 1: IEEE power save mode,
> 
> 2: deep sleep power save mode,
> 
> 3: IEEE power save and deep sleep mode,
> 
> 4: WNM (wireless network management) power save mode,
> 
> 5: WNM power save and deep sleep mode.

#### uint8\_t \_NCP\_CMD\_CONNECT\_STAT::uap\_conn\_stat

> UAP connection status.

#### uint8\_t \_NCP\_CMD\_CONNECT\_STAT::sta\_conn\_stat

> STA connection status.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_CSI Struct Reference

### Data Fields

  - wlan\_csi\_config\_params\_t csi\_para

### Detailed Description

This structure is used for CSI configuration.

### Field Documentation

#### wlan\_csi\_config\_params\_t \_NCP\_CMD\_CSI::csi\_para

> CSI (channel state information) configuration.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t tx\_max\_len

  - uint16\_t tx\_max\_time

  - uint16\_t rx\_max\_len

  - uint16\_t rx\_max\_time

### Detailed Description

This structure contains the value of the Bluetooth LE data length change
event which notifies the Host of a change to either the maximum LL Data
PDU Payload length or the maximum transmission time of packets
containing LL Data PDUs in either direction.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV::tx\_max\_len

> Maximum data length of TX connection

#### uint16\_t \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV::tx\_max\_time

> Maximum time of TX connection

#### uint16\_t \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV::rx\_max\_len

> Maximum length of RX connection

#### uint16\_t \_NCP\_CMD\_DATA\_LEN\_UPDATE\_EV::rx\_max\_time

> Maximum time of RX connection

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_DATE\_TIME Struct Reference

### Data Fields

  - uint32\_t action

  - wlan\_date\_time\_t date\_time

### Detailed Description

This structure is used for date and time configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_DATE\_TIME::action

> Action type,
> 
> 0: get,
> 
> 1: set.

#### wlan\_date\_time\_t \_NCP\_CMD\_DATE\_TIME::date\_time

> Date and time structure.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_DEEP\_SLEEP\_PS Struct Reference

### Data Fields

  - int enable

### Detailed Description

This structure is used for deep sleep power save mode flag
configuration.

### Field Documentation

#### int \_NCP\_CMD\_DEEP\_SLEEP\_PS::enable

> Enable deep sleep power save mode flag,
> 
> 0: disable,
> 
> 1: enable.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_ED\_MAC Struct Reference

### Data Fields

  - uint8\_t action

  - uint16\_t ed\_ctrl\_2g

  - uint16\_t ed\_offset\_2g

  - uint16\_t ed\_ctrl\_5g

  - uint16\_t ed\_offset\_5g

### Detailed Description

This structure is used for EDMAC (energy detect media access control)
mode configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_ED\_MAC::action

> Action,
> 
> 0: get,
> 
> 1: set.

#### uint16\_t \_NCP\_CMD\_ED\_MAC::ed\_ctrl\_2g

> Enable/disable EU (Encrypt unit) adaptivity for 2.4GHz band.

#### uint16\_t \_NCP\_CMD\_ED\_MAC::ed\_offset\_2g

> Energy detect threshold for 2.4GHz band.

#### uint16\_t \_NCP\_CMD\_ED\_MAC::ed\_ctrl\_5g

> Enable/disable EU adaptivity for 5GHz band.

#### uint16\_t \_NCP\_CMD\_ED\_MAC::ed\_offset\_5g

> Energy detect threshold for 5GHz band.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_ENCRYPTION Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t addr \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for NCP Bluetooth LE encryption.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_ENCRYPTION::type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_ENCRYPTION::addr\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_EU\_CRYPRTO Struct Reference

### Data Fields

  - uint8\_t enc

### Detailed Description

This structure is used for EU (encryption unit) crypto configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_EU\_CRYPRTO::enc

> 0: decrypt,
> 
> 1: encrypt.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_FW\_VERSION Struct Reference

### Data Fields

  - char driver\_ver\_str \[MLAN\_MAX\_DRIVER\_VER\_STR\_LEN\]

  - char fw\_ver\_str \[MLAN\_MAX\_VER\_STR\_LEN\]

### Detailed Description

This structure is used to store Wi-Fi driver and firmware version.

### Field Documentation

#### char \_NCP\_CMD\_FW\_VERSION::driver\_ver\_str\[MLAN\_MAX\_DRIVER\_VER\_STR\_LEN\]

> Driver version string.

#### char \_NCP\_CMD\_FW\_VERSION::fw\_ver\_str\[MLAN\_MAX\_VER\_STR\_LEN\]

> Firmware version string.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_GET\_CURRENT\_NETWORK Struct Reference

### Data Fields

  - NCP\_WLAN\_NETWORK **sta\_network**

### Detailed Description

This structure is used for store the information about the current
network.

The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

## \_NCP\_CMD\_GET\_MAC\_ADDRESS Struct Reference

### Data Fields

  - uint8\_t uap\_mac \[MLAN\_MAC\_ADDR\_LENGTH\]

  - uint8\_t sta\_mac \[MLAN\_MAC\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for MAC address configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_GET\_MAC\_ADDRESS::uap\_mac\[MLAN\_MAC\_ADDR\_LENGTH\]

> MAC address of UAP.

#### uint8\_t \_NCP\_CMD\_GET\_MAC\_ADDRESS::sta\_mac\[MLAN\_MAC\_ADDR\_LENGTH\]

> MAC address of STA.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_CONNECT\_CFG Struct Reference

### Data Fields

  - int opened\_handle

  - char host \[1\]

### Detailed Description

This structure is used for HTTP connect configuration.

### Field Documentation

#### int \_NCP\_CMD\_HTTP\_CONNECT\_CFG::opened\_handle

> Created HTTP handle.

#### char \_NCP\_CMD\_HTTP\_CONNECT\_CFG::host\[1\]

> HTTP host, the maximum string length is 512.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_DISCONNECT\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

### Detailed Description

This structure is used for HTTP disconnect configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_HTTP\_DISCONNECT\_CFG::handle

> Socket handle index number.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_RECV\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t recv\_size

  - uint32\_t timeout

  - char recv\_data \[1\]

### Detailed Description

This structure is used for HTTP receive data configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_HTTP\_RECV\_CFG::handle

> Socket handle index number.

#### uint32\_t \_NCP\_CMD\_HTTP\_RECV\_CFG::recv\_size

> Size of HTTP received data

#### uint32\_t \_NCP\_CMD\_HTTP\_RECV\_CFG::timeout

> HTTP receive data wait time

#### char \_NCP\_CMD\_HTTP\_RECV\_CFG::recv\_data\[1\]

> Received data buffer.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_REQ\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - char method \[HTTP\_PARA\_LEN\]

  - char uri \[HTTP\_URI\_LEN\]

  - uint32\_t req\_size

  - char req\_data \[1\]

### Detailed Description

This structure is used for HTTP request configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_HTTP\_REQ\_CFG::handle

> Socket handle index number.

#### char \_NCP\_CMD\_HTTP\_REQ\_CFG::method\[HTTP\_PARA\_LEN\]

> Support method is get/delete/put/options. The max length is 16.

#### char \_NCP\_CMD\_HTTP\_REQ\_CFG::uri\[HTTP\_URI\_LEN\]

> HTTP URL, the string max length is 512

#### uint32\_t \_NCP\_CMD\_HTTP\_REQ\_CFG::req\_size

> Size of the request data.

#### char \_NCP\_CMD\_HTTP\_REQ\_CFG::req\_data\[1\]

> Request data.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_REQ\_RESP\_CFG Struct Reference

### Data Fields

  - uint32\_t header\_size

  - char recv\_header \[1\]

### Detailed Description

This structure is used for HTTP request response configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_HTTP\_REQ\_RESP\_CFG::header\_size

> Received header size

#### char \_NCP\_CMD\_HTTP\_REQ\_RESP\_CFG::recv\_header\[1\]

> Received header

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_SETH\_CFG Struct Reference

### Data Fields

  - char name \[SETH\_NAME\_LENGTH\]

  - char value \[SETH\_VALUE\_LENGTH\]

### Detailed Description

This structure is used for HTTP header configuration.

### Field Documentation

#### char \_NCP\_CMD\_HTTP\_SETH\_CFG::name\[SETH\_NAME\_LENGTH\]

> HTTP header segment name.

#### char \_NCP\_CMD\_HTTP\_SETH\_CFG::value\[SETH\_VALUE\_LENGTH\]

> HTTP header segment value.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_UNSETH\_CFG Struct Reference

### Data Fields

  - char name \[SETH\_NAME\_LENGTH\]

### Detailed Description

This structure is used for unset HTTP header segment name configuration.

### Field Documentation

#### char \_NCP\_CMD\_HTTP\_UNSETH\_CFG::name\[SETH\_NAME\_LENGTH\]

> HTTP header segment name.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_HTTP\_UPG\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - char uri \[HTTP\_URI\_LEN\]

  - char protocol \[HTTP\_PARA\_LEN\]

### Detailed Description

This structure is used for HTTP upgrade configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_HTTP\_UPG\_CFG::handle

> Socket handle index number.

#### char \_NCP\_CMD\_HTTP\_UPG\_CFG::uri\[HTTP\_URI\_LEN\]

> Web socket URI

#### char \_NCP\_CMD\_HTTP\_UPG\_CFG::protocol\[HTTP\_PARA\_LEN\]

> Protocol

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_IEEE\_PS Struct Reference

### Data Fields

  - int enable

### Detailed Description

This structure is used for IEEE power save mode configuration.

### Field Documentation

#### int \_NCP\_CMD\_IEEE\_PS::enable

> Enable IEEE power save mode flag,
> 
> 0: disable,
> 
> 1: enable.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_MAC\_ADDRESS Struct Reference

### Data Fields

  - uint8\_t mac\_addr \[MLAN\_MAC\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for MAC address configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_MAC\_ADDRESS::mac\_addr\[MLAN\_MAC\_ADDR\_LENGTH\]

> MAC address.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_MBO\_ENABLE Struct Reference

### Data Fields

  - int enable

### Detailed Description

This structure is used for MBO (multi band operation) flag
configuration.

### Field Documentation

#### int \_NCP\_CMD\_MBO\_ENABLE::enable

> Enable/disable MBO flag,
> 
> 0: disable,
> 
> 1: enable.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_MBO\_SET\_CELL\_CAPA Struct Reference

### Data Fields

  - uint8\_t cell\_capa

### Detailed Description

This structure is used for MBO cellular data capabilities configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_MBO\_SET\_CELL\_CAPA::cell\_capa

> MBO cellular data capabilities,
> 
> 1: cellular data connection available,
> 
> 2: cellular data connection not available,
> 
> 3: not cellular capable (default value).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_MBO\_SET\_OCE Struct Reference

### Data Fields

  - uint8\_t oce

### Detailed Description

This structure is used for MBO OCE (optimized connectivity experience)
configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_MBO\_SET\_OCE::oce

> Enable OCE features,
> 
> 1: Enable OCE in non-AP STA mode,
> 
> 2: Enable OCE in STA-CFON mode.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_MDNS\_QUERY Struct Reference

### Data Fields

  - uint8\_t qtype

  - union {

  -   QUERY\_PTR\_CFG **ptr\_cfg**

  -   QUERY\_A\_CFG **a\_cfg**

  - } **Q**

### Detailed Description

This structure is used to store MDNS query results.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_MDNS\_QUERY::qtype

> Query type, PTR (pointer records), SRV (service records), A (Ipv4),
> AAAA (IPv6).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_MEM\_STAT Struct Reference

### Data Fields

  - uint32\_t free\_heap\_size

  - uint32\_t minimum\_ever\_free\_heap\_size

### Detailed Description

This structure is used for memory status configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_MEM\_STAT::free\_heap\_size

> Size of free heap.

#### uint32\_t \_NCP\_CMD\_MEM\_STAT::minimum\_ever\_free\_heap\_size

> Minimum size of ever free heap.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NEIGHBOR\_REQ Struct Reference

### Data Fields

  - SSID\_ParamSet\_t ssid\_tlv

### Detailed Description

This structure is used for neighbor request.

### Field Documentation

#### SSID\_ParamSet\_t \_NCP\_CMD\_NEIGHBOR\_REQ::ssid\_tlv

> SSID parameter set.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NET\_MONITOR Struct Reference

### Data Fields

  - NCP\_WLAN\_NET\_MONITOR\_PARA monitor\_para

### Detailed Description

This structure is used for network monitor configuration.

### Field Documentation

#### NCP\_WLAN\_NET\_MONITOR\_PARA \_NCP\_CMD\_NET\_MONITOR::monitor\_para

> Monitor configuration.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_ADD Struct Reference

### Data Fields

  - char name \[WLAN\_NETWORK\_NAME\_MAX\_LENGTH\]

  - uint32\_t tlv\_buf\_len

  - uint8\_t tlv\_buf \[1\]

### Detailed Description

This structure is used to store the information about the network to be
added.

### Field Documentation

#### char \_NCP\_CMD\_NETWORK\_ADD::name\[WLAN\_NETWORK\_NAME\_MAX\_LENGTH\]

> Network name string

#### uint32\_t \_NCP\_CMD\_NETWORK\_ADD::tlv\_buf\_len

> Length of TLVs sent in command starting at TLV buffer

#### uint8\_t \_NCP\_CMD\_NETWORK\_ADD::tlv\_buf\[1\]

> Payload of "wlan-add" command, includes:
> 
> SSID TLV, SSID\_ParamSet\_t
> 
> BSSID TLV, BSSID\_ParamSet\_t
> 
> BSS role TLV, BSSRole\_ParamSet\_t
> 
> Channel TLV, Channel\_ParamSet\_t
> 
> ACS band TLV, ACSBand\_ParamSet\_t
> 
> IP address TLV, IP\_ParamSet\_t
> 
> Security TLV, Security\_ParamSet\_t
> 
> DTIM period TLV, DTIM\_ParamSet\_t
> 
> CAPA TLV, CAPA\_ParamSet\_t
> 
> PMF TLV, PMF\_ParamSet\_t

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_ADDRESS Struct Reference

### Data Fields

  - uint8\_t sta\_conn\_stat

  - NCP\_WLAN\_NETWORK sta\_network

### Detailed Description

This structure is used for STA network address configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_NETWORK\_ADDRESS::sta\_conn\_stat

> Station connection status

#### NCP\_WLAN\_NETWORK \_NCP\_CMD\_NETWORK\_ADDRESS::sta\_network

> Network information for the station

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_INFO Struct Reference

### Data Fields

  - uint8\_t uap\_conn\_stat

  - uint8\_t sta\_conn\_stat

  - NCP\_WLAN\_NETWORK uap\_network

  - NCP\_WLAN\_NETWORK sta\_network

### Detailed Description

This structure is used for network information configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_NETWORK\_INFO::uap\_conn\_stat

> UAP connection status

#### uint8\_t \_NCP\_CMD\_NETWORK\_INFO::sta\_conn\_stat

> Station connection status

#### NCP\_WLAN\_NETWORK \_NCP\_CMD\_NETWORK\_INFO::uap\_network

> Network information for the UAP

#### NCP\_WLAN\_NETWORK \_NCP\_CMD\_NETWORK\_INFO::sta\_network

> Network information for the station

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_LIST Struct Reference

### Data Fields

  - uint8\_t count

  - NCP\_WLAN\_NETWORK net\_list \[NCP\_WLAN\_KNOWN\_NETWORKS\]

### Detailed Description

This structure is used for network list configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_NETWORK\_LIST::count

> Network number

#### NCP\_WLAN\_NETWORK \_NCP\_CMD\_NETWORK\_LIST::net\_list\[NCP\_WLAN\_KNOWN\_NETWORKS\]

> Network information list.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_REMOVE Struct Reference

### Data Fields

  - char name \[WLAN\_NETWORK\_NAME\_MAX\_LENGTH+1\]

  - int8\_t remove\_state

### Detailed Description

This structure is used to store the information about the network to be
removed.

### Field Documentation

#### char \_NCP\_CMD\_NETWORK\_REMOVE::name\[WLAN\_NETWORK\_NAME\_MAX\_LENGTH+1\]

> Name of the network to remove.

#### int8\_t \_NCP\_CMD\_NETWORK\_REMOVE::remove\_state

> Remove status, WM\_SUCCESS means removed successfully, otherwise
> removed failed

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_START Struct Reference

### Data Fields

  - char name \[WLAN\_NETWORK\_NAME\_MAX\_LENGTH\]

  - char ssid \[IEEEtypes\_SSID\_SIZE+1\]

### Detailed Description

This structure is used to store the information about the UAP to be
started.

### Field Documentation

#### char \_NCP\_CMD\_NETWORK\_START::name\[WLAN\_NETWORK\_NAME\_MAX\_LENGTH\]

> Network name string.

#### char \_NCP\_CMD\_NETWORK\_START::ssid\[IEEEtypes\_SSID\_SIZE+1\]

> SSID (service set identifier).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_NETWORK\_UAP\_STA\_LIST Struct Reference

### Data Fields

  - uint16\_t sta\_count

  - wifi\_sta\_info\_t info \[MAX\_NUM\_CLIENTS\]

### Detailed Description

This structure is used to store a collection of STAs information, those
are connected to the UAP.

### Field Documentation

#### uint16\_t \_NCP\_CMD\_NETWORK\_UAP\_STA\_LIST::sta\_count

> Number of STAs connected to the UAP.

#### wifi\_sta\_info\_t \_NCP\_CMD\_NETWORK\_UAP\_STA\_LIST::info\[MAX\_NUM\_CLIENTS\]

> Information on the STAs connected to the UAP.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_PHY\_UPDATE\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t tx\_phy

  - uint8\_t rx\_phy

### Detailed Description

This structure contains the value of the Bluetooth LE PHY update
complete event which indicate that the controller has changed the
transmitter PHY or receiver PHY in use.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_PHY\_UPDATE\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_PHY\_UPDATE\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t \_NCP\_CMD\_PHY\_UPDATE\_EV::tx\_phy

> TX physical
> 
> 1: 1M PHY
> 
> 2: 2M PHY
> 
> 4: Coded PHY

#### uint8\_t \_NCP\_CMD\_PHY\_UPDATE\_EV::rx\_phy

> RX physical
> 
> 1: 1M PHY
> 
> 2: 2M PHY
> 
> 4: Coded PHY

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_POWERMGMT\_MEF Struct Reference

### Data Fields

  - int type

  - uint8\_t action

### Detailed Description

This structure is used for power management MEF (memory efficient
filtering) configuration.

### Field Documentation

#### int \_NCP\_CMD\_POWERMGMT\_MEF::type

> Type,
> 
> 0: delete,
> 
> 1: ping,
> 
> 2: ARP (address resolution protocol).
> 
> 3: multicast,
> 
> 4: Ipv6 NS.

#### uint8\_t \_NCP\_CMD\_POWERMGMT\_MEF::action

> Action,
> 
> 0: discard and do not wake up the host,
> 
> 1: discard and wake up the host,
> 
> 3: allow and wake up the host.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_POWERMGMT\_QOSINFO Struct Reference

### Data Fields

  - uint8\_t qos\_info

  - uint8\_t action

### Detailed Description

This structure is used for QOS (quality of service) operation
configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_POWERMGMT\_QOSINFO::qos\_info

> QOS information.

#### uint8\_t \_NCP\_CMD\_POWERMGMT\_QOSINFO::action

> Action,
> 
> 0: get,
> 
> 1: set.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_POWERMGMT\_SLEEP\_PERIOD Struct Reference

### Data Fields

  - uint32\_t period

  - uint8\_t action

### Detailed Description

This structure is used for power management sleep period configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_POWERMGMT\_SLEEP\_PERIOD::period

> Power management sleep period

#### uint8\_t \_NCP\_CMD\_POWERMGMT\_SLEEP\_PERIOD::action

> Action,
> 
> 0: get,
> 
> 1: set.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_POWERMGMT\_SUSPEND Struct Reference

### Data Fields

  - int mode

### Detailed Description

This structure is used for suspend mode of power management.

### Field Documentation

#### int \_NCP\_CMD\_POWERMGMT\_SUSPEND::mode

> Suspend mode,
> 
> 1: PM1,
> 
> 2: PM2,
> 
> 3: PM3.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_POWERMGMT\_UAPSD Struct Reference

### Data Fields

  - int enable

### Detailed Description

This structure is use for UAPSD (unscheduled automatic power save
delivery) flag configuration.

### Field Documentation

#### int \_NCP\_CMD\_POWERMGMT\_UAPSD::enable

> Enable flag,
> 
> 0: disable UAPSD,
> 
> 1: enable UAPSD.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_POWERMGMT\_WOWLAN\_CFG Struct Reference

### Data Fields

  - uint8\_t is\_mef

  - uint8\_t wake\_up\_conds

### Detailed Description

This structure is used for WOWLAN (wake on wireless LAN) power
management configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_POWERMGMT\_WOWLAN\_CFG::is\_mef

> Type,
> 
> 0x0: WOWLAN
> 
> 0x1: MEF.

#### uint8\_t \_NCP\_CMD\_POWERMGMT\_WOWLAN\_CFG::wake\_up\_conds

> Wakeup condition bitmap, value for default WOWLAN (wake on wireless
> LAN) conditions only.
> 
> bit 0: WAKE\_ON\_ALL\_BROADCAST,
> 
> bit 1: WAKE\_ON\_UNICAST,
> 
> bit 2: WAKE\_ON\_MAC\_EVENT,
> 
> bit 3: WAKE\_ON\_MULTICAST,
> 
> bit 4: WAKE\_ON\_ARP\_BROADCAST,
> 
> bit 6: WAKE\_ON\_MGMT\_FRAME,
> 
> all bit 0 discard and not wakeup host.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_REGION\_CODE Struct Reference

### Data Fields

  - uint32\_t action

  - uint32\_t region\_code

### Detailed Description

This structure is used for region code configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_REGION\_CODE::action

> Action,
> 
> 0: get,
> 
> 1: set.

#### uint32\_t \_NCP\_CMD\_REGION\_CODE::region\_code

> Region code,
> 
> 0x00:"WW" World Wide,
> 
> 0x10:"US" US FCC,
> 
> 0x20:"CA" IC Canada,
> 
> 0x10:"SG" Singapore,
> 
> 0x30:"EU" ETSI,
> 
> 0x30:"AU" Australia,
> 
> 0x30:"KR" Republic Of Korea,
> 
> 0x32:"FR" France,
> 
> 0xFF:"JP" Japan,
> 
> 0x50:"CN" China.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_REGISTER\_ACCESS Struct Reference

### Data Fields

  - uint8\_t action

  - uint8\_t type

  - uint32\_t offset

  - uint32\_t value

### Detailed Description

This structure is used for register access configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_REGISTER\_ACCESS::action

> Action,
> 
> 0: get,
> 
> 1: set.

#### uint8\_t \_NCP\_CMD\_REGISTER\_ACCESS::type

> 1: MAC,
> 
> 2: BBP,
> 
> 3: RF,
> 
> 4: CAU

#### uint32\_t \_NCP\_CMD\_REGISTER\_ACCESS::offset

> Offset of register.

#### uint32\_t \_NCP\_CMD\_REGISTER\_ACCESS::value

> Value of register.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_BAND Struct Reference

### Data Fields

  - uint8\_t band

### Detailed Description

This structure is used for RF band configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_BAND::band

> Band mode,
> 
> 0: 2.4GHz,
> 
> 1: 5GHz.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_BANDWIDTH Struct Reference

### Data Fields

  - uint8\_t bandwidth

### Detailed Description

This structure is used for RF bandwidth configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_BANDWIDTH::bandwidth

> Bandwidth,
> 
> 0: 20MHz;
> 
> 1: 40MHz;
> 
> 4: 80MHz.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_CHANNEL Struct Reference

### Data Fields

  - uint8\_t channel

### Detailed Description

This structure is used for RF channel number configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_CHANNEL::channel

> 2.4GHz channel numbers or 5GHz channel numbers.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_PER Struct Reference

### Data Fields

  - uint32\_t rx\_tot\_pkt\_count

  - uint32\_t rx\_mcast\_bcast\_count

  - uint32\_t rx\_pkt\_fcs\_error

### Detailed Description

This structure is used for RF per command configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_RF\_PER::rx\_tot\_pkt\_count

> Sum of RX packet count.

#### uint32\_t \_NCP\_CMD\_RF\_PER::rx\_mcast\_bcast\_count

> Sum of RX multicast/broadcast packet count.

#### uint32\_t \_NCP\_CMD\_RF\_PER::rx\_pkt\_fcs\_error

> Sum of RX packets with FCS (frame check sequence) error count.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_RADIO\_MODE Struct Reference

### Data Fields

  - uint8\_t radio\_mode

### Detailed Description

This structure is used for RF radio mode configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_RADIO\_MODE::radio\_mode

> Radio mode,
> 
> 0: set radio in power down mode,
> 
> 3: sets radio in 5GHz band, 1X1 mode(path A),
> 
> 11: sets radio in 2.4GHz band, 1X1 mode(path A).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_RX\_ANTENNA Struct Reference

### Data Fields

  - uint8\_t ant

### Detailed Description

This structure is used for RF RX antenna configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_RX\_ANTENNA::ant

> Antenna type,
> 
> 1: main antenna,
> 
> 2: aux antenna.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_TX\_ANTENNA Struct Reference

### Data Fields

  - uint8\_t ant

### Detailed Description

This structure is used for RF TX antenna configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_TX\_ANTENNA::ant

> Antenna type,
> 
> 1: main antenna,
> 
> 2: aux antenna.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_TX\_CONT\_MODE Struct Reference

### Data Fields

  - uint32\_t enable\_tx

  - uint32\_t cw\_mode

  - uint32\_t payload\_pattern

  - uint32\_t cs\_mode

  - uint32\_t act\_sub\_ch

  - uint32\_t tx\_rate

### Detailed Description

This structure is used for RF TX continuous configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_RF\_TX\_CONT\_MODE::enable\_tx

> Enable TX flag,
> 
> 0: disable TX,
> 
> 1: enable TX.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_CONT\_MODE::cw\_mode

> Enable CW (continuous wave) mode flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_CONT\_MODE::payload\_pattern

> Payload pattern, 0 to 0xFFFFFFF (enter hexadecimal value).

#### uint32\_t \_NCP\_CMD\_RF\_TX\_CONT\_MODE::cs\_mode

> Enable CS mode flag, applicable only when continuous wave is disabled,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_CONT\_MODE::act\_sub\_ch

> Active sub channel,
> 
> 0: low,
> 
> 1: upper,
> 
> 3: both.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_CONT\_MODE::tx\_rate

> Transmit data rate, rate index corresponding to legacy/HT (802.11n
> high throughput)/VHT (802.11ac very high throughput) rates.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_TX\_FRAME Struct Reference

### Data Fields

  - uint32\_t enable

  - uint32\_t data\_rate

  - uint32\_t frame\_pattern

  - uint32\_t frame\_length

  - uint32\_t adjust\_burst\_sifs

  - uint32\_t burst\_sifs\_in\_us

  - uint32\_t short\_preamble

  - uint32\_t act\_sub\_ch

  - uint32\_t short\_gi

  - uint32\_t adv\_coding

  - uint32\_t tx\_bf

  - uint32\_t gf\_mode

  - uint32\_t stbc

  - uint8\_t bssid \[MLAN\_MAC\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for RF TX frame configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::enable

> Enable flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::data\_rate

> Transmit data rate, rate index corresponding to legacy/HT/VHT rates.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::frame\_pattern

> Payload pattern, (0 to 0xFFFFFFFF) (Enter hexadecimal value)

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::frame\_length

> Payload length, (1 to 0x400) (Enter hexadecimal value)

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::adjust\_burst\_sifs

> Adjust burst SIFS3 gap enable flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::burst\_sifs\_in\_us

> Burst SIFS (short interframe space) in us (0 to 255us)

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::short\_preamble

> Enable short preamble flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::act\_sub\_ch

> Enable active sub channel flag,
> 
> 0: low,
> 
> 1: upper,
> 
> 3: both.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::short\_gi

> Enable short GI (guard interval) flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::adv\_coding

> Enable advanced coding flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::tx\_bf

> Enable beamforming flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::gf\_mode

> Enable green field mode flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint32\_t \_NCP\_CMD\_RF\_TX\_FRAME::stbc

> Enable STBC (space time block coding) flag,
> 
> 0: disable,
> 
> 1: enable.

#### uint8\_t \_NCP\_CMD\_RF\_TX\_FRAME::bssid\[MLAN\_MAC\_ADDR\_LENGTH\]

> BSSID string.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RF\_TX\_POWER Struct Reference

### Data Fields

  - uint8\_t power

  - uint8\_t mod

  - uint8\_t path\_id

### Detailed Description

This structure is used for RF TX power configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_RF\_TX\_POWER::power

> Power value: 0\~24 dBm.

#### uint8\_t \_NCP\_CMD\_RF\_TX\_POWER::mod

> Modulation mode,
> 
> 0: CCK (complementary code keying),
> 
> 1: OFDM (orthogonal frequency division multiplexing),
> 
> 2: MCS.

#### uint8\_t \_NCP\_CMD\_RF\_TX\_POWER::path\_id

> Path ID,
> 
> 0: path A,
> 
> 1: path B,
> 
> 2: path A+B.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_ROAMING Struct Reference

### Data Fields

  - uint32\_t enable

  - uint8\_t rssi\_threshold

### Detailed Description

This structure is used for roaming configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_ROAMING::enable

> STA roaming enable flag,
> 
> 1: enable roaming,
> 
> 0: disable roaming.

#### uint8\_t \_NCP\_CMD\_ROAMING::rssi\_threshold

> When the RSSI of the AP currently connected to the STA is lower than
> this threshold, the STA begins to scan the environment to find APs
> with better signals for roaming.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_RSSI Struct Reference

### Data Fields

  - NCP\_WLAN\_RSSI\_INFO\_T rssi\_info

### Detailed Description

This structure is used for RSSI configuration.

### Field Documentation

#### NCP\_WLAN\_RSSI\_INFO\_T \_NCP\_CMD\_RSSI::rssi\_info

> RSSI information.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SCAN\_NETWORK\_INFO Struct Reference

### Data Fields

  - uint32\_t res\_cnt

  - NCP\_WLAN\_SCAN\_RESULT res \[CONFIG\_MAX\_AP\_ENTRIES\]

### Detailed Description

This structure is used to store the information of all scanned APs.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SCAN\_NETWORK\_INFO::res\_cnt

> Number of scanned APs.

#### NCP\_WLAN\_SCAN\_RESULT \_NCP\_CMD\_SCAN\_NETWORK\_INFO::res\[CONFIG\_MAX\_AP\_ENTRIES\]

> Information of each scanned AP.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SCAN\_START Struct Reference

### Data Fields

  - uint8\_t type

### Detailed Description

This structure is used for NCP Bluetooth LE start scanning.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SCAN\_START::type

> scan start
> 
> 0: active scan
> 
> 1: passive scan

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SERVICE\_ADD Struct Reference

### Data Fields

  - uint32\_t tlv\_buf\_len

  - uint8\_t tlv\_buf \[1\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE add service command.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SERVICE\_ADD::tlv\_buf\_len

> the buffer length of TLV (type length value)

#### uint8\_t \_NCP\_CMD\_SERVICE\_ADD::tlv\_buf\[1\]

> add service TLV, gatt\_add\_service\_cmd\_t
> 
> add characteristic TLV, gatt\_add\_characteristic\_cmd\_t
> 
> add descriptor TLV, gatt\_add\_descriptor\_cmd\_t
> 
> add include service TLV, gatt\_add\_included\_service\_cmd\_t (to be
> added in the future)
> 
> start host service TLV, gatt\_start\_service\_cmd\_t

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SERVICE\_DISC Struct Reference

### Data Fields

  - uint32\_t tlv\_buf\_len

  - uint8\_t tlv\_buf \[1\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
service discovery data.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SERVICE\_DISC::tlv\_buf\_len

> the buffer length of TLV (type length value)

#### uint8\_t \_NCP\_CMD\_SERVICE\_DISC::tlv\_buf\[1\]

> discovery primary service TLV, gatt\_disc\_prim\_uuid\_cmd\_t
> 
> discovery characteristic TLV, gatt\_disc\_chrc\_uuid\_cmd\_t
> 
> discovery descriptor TLV, gatt\_disc\_desc\_uuid\_cmd\_t

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_ADDR Struct Reference

### Data Fields

  - uint8\_t addr \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for NCP Bluetooth LE set device address.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SET\_ADDR::addr\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_ADV\_DATA Struct Reference

### Data Fields

  - uint8\_t adv\_length

  - uint8\_t adv\_data \[\]

### Detailed Description

This structure is used for NCP Bluetooth LE set advertising data.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SET\_ADV\_DATA::adv\_length

> advertising data length

#### uint8\_t \_NCP\_CMD\_SET\_ADV\_DATA::adv\_data\[\]

> advertising data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_DATA\_LEN Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t time\_flag

  - uint16\_t tx\_max\_len

  - uint16\_t tx\_max\_time

### Detailed Description

This structure is used for NCP Bluetooth LE set data length.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SET\_DATA\_LEN::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_SET\_DATA\_LEN::address\[NCP\_BLE\_ADDR\_LENGTH\]

> remote address

#### uint8\_t \_NCP\_CMD\_SET\_DATA\_LEN::time\_flag

> time flag

#### uint16\_t \_NCP\_CMD\_SET\_DATA\_LEN::tx\_max\_len

> max transmit data length

#### uint16\_t \_NCP\_CMD\_SET\_DATA\_LEN::tx\_max\_time

> max transmit time

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_NAME Struct Reference

### Data Fields

  - uint8\_t name \[33\]

### Detailed Description

This structure is used for NCP Bluetooth LE set device name.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SET\_NAME::name\[33\]

> device name

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_PHY Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t options

  - uint8\_t pref\_tx\_phy

  - uint8\_t pref\_rx\_phy

### Detailed Description

This structure is used for NCP Bluetooth LE set PHY.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SET\_PHY::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_CMD\_SET\_PHY::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_CMD\_SET\_PHY::options

> Connection PHY options
> 
> 0: BT\_CONN\_LE\_PHY\_OPT\_NONE (Convenience value when no options are
> specified)
> 
> bit0: BT\_CONN\_LE\_PHY\_OPT\_CODED\_S2 (LE Coded using S=2 coding
> preferred when transmitting)
> 
> bit1: BT\_CONN\_LE\_PHY\_OPT\_CODED\_S8 (LE Coded using S=8 coding
> preferred when transmitting)

#### uint8\_t \_NCP\_CMD\_SET\_PHY::pref\_tx\_phy

> Bitmask of preferred transmit PHYs
> 
> 0: BT\_GAP\_LE\_PHY\_NONE (Convenience macro for when no PHY is set)
> 
> bit0: BT\_GAP\_LE\_PHY\_1M (LE 1M PHY)
> 
> bit1: BT\_GAP\_LE\_PHY\_2M (LE 2M PHY)
> 
> bit2: BT\_GAP\_LE\_PHY\_CODED (LE Coded PHY)

#### uint8\_t \_NCP\_CMD\_SET\_PHY::pref\_rx\_phy

> Bitmask of preferred receive PHYs
> 
> 0: BT\_GAP\_LE\_PHY\_NONE (Convenience macro for when no PHY is set)
> 
> bit0: BT\_GAP\_LE\_PHY\_1M (LE 1M PHY)
> 
> bit1: BT\_GAP\_LE\_PHY\_2M (LE 2M PHY)
> 
> bit2: BT\_GAP\_LE\_PHY\_CODED (LE Coded PHY)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_POWER\_MODE Struct Reference

### Data Fields

  - uint8\_t mode

### Detailed Description

This structure is used for NCP Bluetooth LE set power mode.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_SET\_POWER\_MODE::mode

> Set narrowband controller power mode.
> 
> auto sleep disable : 0x02
> 
> auto sleep enable : 0x03

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SET\_SCAN\_PARAM Struct Reference

### Data Fields

  - uint32\_t options

  - uint16\_t interval

  - uint16\_t window

### Detailed Description

This structure is used for NCP Bluetooth LE set scan parameter.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SET\_SCAN\_PARAM::options

> bit-field of scanning options.

#### uint16\_t \_NCP\_CMD\_SET\_SCAN\_PARAM::interval

> scan interval (N \* 0.625 ms)

#### uint16\_t \_NCP\_CMD\_SET\_SCAN\_PARAM::window

> scan window (N \* 0.625 ms)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_SOCKET\_ACCEPT\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - int accepted\_handle

### Detailed Description

This structure is used for socket accept configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_ACCEPT\_CFG::handle

> Socket handle index number.

#### int \_NCP\_CMD\_SOCKET\_ACCEPT\_CFG::accepted\_handle

> Accept socket handle.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_BIND\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t port

  - char ip\_addr \[IP\_ADDR\_LEN\]

### Detailed Description

This structure is used for socket bind configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_BIND\_CFG::handle

> Socket handle index number.

#### uint32\_t \_NCP\_CMD\_SOCKET\_BIND\_CFG::port

> socket port number to be bound.

#### char \_NCP\_CMD\_SOCKET\_BIND\_CFG::ip\_addr\[IP\_ADDR\_LEN\]

> IP address to be bound.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_CLOSE\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

### Detailed Description

This structure is used for socket closing configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_CLOSE\_CFG::handle

> Socket handle index number.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_CON\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t port

  - char ip\_addr \[IP\_ADDR\_LEN\]

### Detailed Description

This structure is used for socket connect configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_CON\_CFG::handle

> Socket handle index number

#### uint32\_t \_NCP\_CMD\_SOCKET\_CON\_CFG::port

> Port number

#### char \_NCP\_CMD\_SOCKET\_CON\_CFG::ip\_addr\[IP\_ADDR\_LEN\]

> IP address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_LISTEN\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t number

### Detailed Description

This structure is used for socket listening configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_LISTEN\_CFG::handle

> Socket handle index number

#### uint32\_t \_NCP\_CMD\_SOCKET\_LISTEN\_CFG::number

> Maximum number of sockets to listen to

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_OPEN\_CFG Struct Reference

### Data Fields

  - char socket\_type \[HTTP\_PARA\_LEN\]

  - char domain\_type \[HTTP\_PARA\_LEN\]

  - char protocol \[HTTP\_PARA\_LEN\]

  - uint32\_t opened\_handle

### Detailed Description

This structure is used for socket open configuration.

### Field Documentation

#### char \_NCP\_CMD\_SOCKET\_OPEN\_CFG::socket\_type\[HTTP\_PARA\_LEN\]

> Socket type: raw/UDP/TCP

#### char \_NCP\_CMD\_SOCKET\_OPEN\_CFG::domain\_type\[HTTP\_PARA\_LEN\]

> Domain type

#### char \_NCP\_CMD\_SOCKET\_OPEN\_CFG::protocol\[HTTP\_PARA\_LEN\]

> Protocol.

#### uint32\_t \_NCP\_CMD\_SOCKET\_OPEN\_CFG::opened\_handle

> Handle to open.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_RECEIVE\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t recv\_size

  - uint32\_t timeout

  - char recv\_data \[1\]

### Detailed Description

This structure is used for socket receive configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECEIVE\_CFG::handle

> Peer socket handle.

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECEIVE\_CFG::recv\_size

> Received buffer size, the buffer max length is 4072.

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECEIVE\_CFG::timeout

> Wait time.

#### char \_NCP\_CMD\_SOCKET\_RECEIVE\_CFG::recv\_data\[1\]

> Received data.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t recv\_size

  - uint32\_t timeout

  - char peer\_ip \[IP\_ADDR\_LEN\]

  - uint32\_t peer\_port

  - char recv\_data \[1\]

### Detailed Description

This structure is used for socket recvfrom configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG::handle

> Peer socket handle.

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG::recv\_size

> Size of received buffer, the buffer maximum length is 4072.

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG::timeout

> Wait time.

#### char \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG::peer\_ip\[IP\_ADDR\_LEN\]

> Peer IP address.

#### uint32\_t \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG::peer\_port

> Peer port number.

#### char \_NCP\_CMD\_SOCKET\_RECVFROM\_CFG::recv\_data\[1\]

> Received data.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_SEND\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t size

  - char send\_data \[1\]

### Detailed Description

This structure is used for socket send configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_SEND\_CFG::handle

> Socket handle index number.

#### uint32\_t \_NCP\_CMD\_SOCKET\_SEND\_CFG::size

> Size of send data.

#### char \_NCP\_CMD\_SOCKET\_SEND\_CFG::send\_data\[1\]

> Data buffer to send.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_SOCKET\_SENDTO\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t size

  - char ip\_addr \[IP\_ADDR\_LEN\]

  - uint32\_t port

  - char send\_data \[1\]

### Detailed Description

This structure is used for socket sendto configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_SOCKET\_SENDTO\_CFG::handle

> Socket handle index number.

#### uint32\_t \_NCP\_CMD\_SOCKET\_SENDTO\_CFG::size

> Send data size.

#### char \_NCP\_CMD\_SOCKET\_SENDTO\_CFG::ip\_addr\[IP\_ADDR\_LEN\]

> Peer device IP address, the max length is 16.

#### uint32\_t \_NCP\_CMD\_SOCKET\_SENDTO\_CFG::port

> Peer port number

#### char \_NCP\_CMD\_SOCKET\_SENDTO\_CFG::send\_data\[1\]

> Data buffer to send.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_START\_SERVICE Struct Reference

### Data Fields

  - uint8\_t form\_host

  - uint8\_t svc\_id

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE start service command.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_START\_SERVICE::form\_host

> host status

#### uint8\_t \_NCP\_CMD\_START\_SERVICE::svc\_id

> service ID
> 
> hts
> 
> hrs
> 
> htc
> 
> hrc
> 
> bas

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_CMD\_TEMPERATURE Struct Reference

### Data Fields

  - uint32\_t temp

### Detailed Description

This structure is used to store temperature of the device.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_TEMPERATURE::temp

> Temperature (celsius) value.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_TWT\_REPORT Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t length

  - uint8\_t reserve \[2\]

  - IEEE\_BTWT\_ParamSet\_t info \[6\]

### Detailed Description

This structure is used for TWT report.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_TWT\_REPORT::type

> TWT report type, 0: BTWT id.

#### uint8\_t \_NCP\_CMD\_TWT\_REPORT::length

> TWT report length of value in data.

#### uint8\_t \_NCP\_CMD\_TWT\_REPORT::reserve\[2\]

> Reserved fields.

#### IEEE\_BTWT\_ParamSet\_t \_NCP\_CMD\_TWT\_REPORT::info\[6\]

> TWT report payload for FW response to fill, 4 \* 9bytes.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_TWT\_SETUP Struct Reference

### Data Fields

  - uint8\_t implicit

  - uint8\_t announced

  - uint8\_t trigger\_enabled

  - uint8\_t twt\_info\_disabled

  - uint8\_t negotiation\_type

  - uint8\_t twt\_wakeup\_duration

  - uint8\_t flow\_identifier

  - uint8\_t hard\_constraint

  - uint8\_t twt\_exponent

  - uint16\_t twt\_mantissa

  - uint8\_t twt\_request

### Detailed Description

This structure is used for TWT configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::implicit

> Implicit enable flag,
> 
> 0: TWT session is explicit,
> 
> 1: session is implicit.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::announced

> Announced enable flag,
> 
> 0: unannounced,
> 
> 1: announced TWT.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::trigger\_enabled

> Trigger enable flag,
> 
> 0: Non-Trigger enabled,
> 
> 1: trigger enabled TWT.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::twt\_info\_disabled

> TWT information disable flag,
> 
> 0: TWT info enabled,
> 
> 1: TWT info disabled.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::negotiation\_type

> Negotiation type,
> 
> 0: individual TWT,
> 
> 3: broadcast TWT.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::twt\_wakeup\_duration

> TWT wakeup duration, time after which the TWT requesting STA can
> transition to doze state.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::flow\_identifier

> Flow identifier. range: \[0-7\].

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::hard\_constraint

> Hard constraint,
> 
> 0: FW can tweak the TWT setup parameters if it is rejected by AP.
> 
> 1: Firmware should not tweak any parameters.

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::twt\_exponent

> TWT exponent, range: \[0-63\].

#### uint16\_t \_NCP\_CMD\_TWT\_SETUP::twt\_mantissa

> TWT wake interval mantissa, range: \[0-(2^16-1)\].

#### uint8\_t \_NCP\_CMD\_TWT\_SETUP::twt\_request

> TWT request type,
> 
> 0: REQUEST\_TWT,
> 
> 1: SUGGEST\_TWT.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_TWT\_TEARDOWN Struct Reference

### Data Fields

  - uint8\_t flow\_identifier

  - uint8\_t negotiation\_type

  - uint8\_t teardown\_all\_twt

### Detailed Description

This structure is used for TWT teardown configuration.

### Field Documentation

#### uint8\_t \_NCP\_CMD\_TWT\_TEARDOWN::flow\_identifier

> TWT flow identifier, range from 0 to 7.

#### uint8\_t \_NCP\_CMD\_TWT\_TEARDOWN::negotiation\_type

> Negotiation type.
> 
> 0: Future individual TWT service period start time,
> 
> 1: Next wake TBTT (target beacon transmission time).

#### uint8\_t \_NCP\_CMD\_TWT\_TEARDOWN::teardown\_all\_twt

> Tear down all TWT enable flag.
> 
> 1: To teardown all TWT,
> 
> 0: none to do.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_UAP\_PROV\_SET\_UAPCFG Struct Reference

### Data Fields

  - uint32\_t security\_type

  - char ssid \[WLAN\_NETWORK\_NAME\_MAX\_LENGTH+1\]

  - char uapPass \[NCP\_WLAN\_PASSWORD\_MAX\_LENGTH+1\]

### Detailed Description

This structure is used for provisioning UAP configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_UAP\_PROV\_SET\_UAPCFG::security\_type

> Security type, detail value refer to enum wlan\_security\_type.

#### char \_NCP\_CMD\_UAP\_PROV\_SET\_UAPCFG::ssid\[WLAN\_NETWORK\_NAME\_MAX\_LENGTH+1\]

> SSID string.

#### char \_NCP\_CMD\_UAP\_PROV\_SET\_UAPCFG::uapPass\[NCP\_WLAN\_PASSWORD\_MAX\_LENGTH+1\]

> Password string.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_WEBSOCKET\_RECV\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - uint32\_t recv\_size

  - uint32\_t timeout

  - uint32\_t fin

  - char recv\_data \[1\]

### Detailed Description

This structure is used for web socket receive configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_WEBSOCKET\_RECV\_CFG::handle

> Web socket handle.

#### uint32\_t \_NCP\_CMD\_WEBSOCKET\_RECV\_CFG::recv\_size

> Size of received data, the maximum length is 4068.

#### uint32\_t \_NCP\_CMD\_WEBSOCKET\_RECV\_CFG::timeout

> Wait time, timeout if no data is received from the peer device within
> a period of time.

#### uint32\_t \_NCP\_CMD\_WEBSOCKET\_RECV\_CFG::fin

> FIN flag is used to terminate the TCP connection, If the value is 1,
> the TCP connection is disconnected.

#### char \_NCP\_CMD\_WEBSOCKET\_RECV\_CFG::recv\_data\[1\]

> Data buffer used to store the packets received from the web socket.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_WEBSOCKET\_SEND\_CFG Struct Reference

### Data Fields

  - uint32\_t handle

  - char type \[HTTP\_PARA\_LEN\]

  - uint32\_t size

  - char send\_data \[1\]

### Detailed Description

This structure is used for web socket send configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_WEBSOCKET\_SEND\_CFG::handle

> Socket handle index number

#### char \_NCP\_CMD\_WEBSOCKET\_SEND\_CFG::type\[HTTP\_PARA\_LEN\]

> Data type.

#### uint32\_t \_NCP\_CMD\_WEBSOCKET\_SEND\_CFG::size

> Size of send data.

#### char \_NCP\_CMD\_WEBSOCKET\_SEND\_CFG::send\_data\[1\]

> Data to be send.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_WLAN\_CONN Struct Reference

### Data Fields

  - char name \[WLAN\_NETWORK\_NAME\_MAX\_LENGTH\]

  - uint32\_t ip

  - char ssid \[IEEEtypes\_SSID\_SIZE+1\]

### Detailed Description

This structure is used for Wi-Fi connection configuration.

### Field Documentation

#### char \_NCP\_CMD\_WLAN\_CONN::name\[WLAN\_NETWORK\_NAME\_MAX\_LENGTH\]

> Wi-Fi network profile name.

#### uint32\_t \_NCP\_CMD\_WLAN\_CONN::ip

> IP address of STA after the connection is successful.

#### char \_NCP\_CMD\_WLAN\_CONN::ssid\[IEEEtypes\_SSID\_SIZE+1\]

> SSID of the connected Wi-Fi network.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_WPS\_GEN\_PIN Struct Reference

### Data Fields

  - uint32\_t pin

### Detailed Description

This structure is used for generated pin code configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_WPS\_GEN\_PIN::pin

> WPS (Wi-Fi protected setup) pin code.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_CMD\_WPS\_PIN Struct Reference

### Data Fields

  - uint32\_t pin

### Detailed Description

This structure is used for pin code configuration.

### Field Documentation

#### uint32\_t \_NCP\_CMD\_WPS\_PIN::pin

> Pin code of WPS (Wi-Fi protected setup).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_DEVICE\_ADV\_REPORT\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - int8\_t rssi

  - uint8\_t flags

  - uint16\_t eir\_data\_len

  - uint8\_t eir\_data \[\]

### Detailed Description

This structure contains the value of the Bluetooth LE advertising report
event which indicates one or more Bluetooth devices have responded to an
active scan or have broadcast advertisements that were received during a
passive scan.

### Field Documentation

#### uint8\_t \_NCP\_DEVICE\_ADV\_REPORT\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_DEVICE\_ADV\_REPORT\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### int8\_t \_NCP\_DEVICE\_ADV\_REPORT\_EV::rssi

> advertising packet RSSI

#### uint8\_t \_NCP\_DEVICE\_ADV\_REPORT\_EV::flags

> Bluetooth LE device found flag
> 
> 0x01: GAP\_DEVICE\_FOUND\_FLAG\_RSSI
> 
> 0x02: GAP\_DEVICE\_FOUND\_FLAG\_AD
> 
> 0x04: GAP\_DEVICE\_FOUND\_FLAG\_SD

#### uint16\_t \_NCP\_DEVICE\_ADV\_REPORT\_EV::eir\_data\_len

> EIR data length

#### uint8\_t \_NCP\_DEVICE\_ADV\_REPORT\_EV::eir\_data\[\]

> EIR data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DEVICE\_CONNECTED\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t interval

  - uint16\_t latency

  - uint16\_t timeout

### Detailed Description

This structure contains the value of the Bluetooth LE connection
complete event which indicates a new connection has been created.

### Field Documentation

#### uint8\_t \_NCP\_DEVICE\_CONNECTED\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_DEVICE\_CONNECTED\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_DEVICE\_CONNECTED\_EV::interval

> connection interval

#### uint16\_t \_NCP\_DEVICE\_CONNECTED\_EV::latency

> connection latency
> 
> Maximum peripheral latency for the connection in number of subrated
> connection events

#### uint16\_t \_NCP\_DEVICE\_CONNECTED\_EV::timeout

> connection timeout
> 
> Supervision timeout for the Bluetooth LE link

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DEVICE\_DISCONNECTED\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the value of the Bluetooth LE disconnection
complete event which indicates a connection is terminated.

### Field Documentation

#### uint8\_t \_NCP\_DEVICE\_DISCONNECTED\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_DEVICE\_DISCONNECTED\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DISC\_ALL\_DESC\_RP Struct Reference

### Data Fields

  - uint8\_t descriptors\_count

  - GATT\_DESCRIPTOR\_T descriptors \[MAX\_SUPPORT\_SERVICE\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discover
descriptors command.

### Field Documentation

#### uint8\_t \_NCP\_DISC\_ALL\_DESC\_RP::descriptors\_count

> descriptors count

#### GATT\_DESCRIPTOR\_T \_NCP\_DISC\_ALL\_DESC\_RP::descriptors\[MAX\_SUPPORT\_SERVICE\]

> descriptors information

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DISC\_CHRC\_RP Struct Reference

### Data Fields

  - uint8\_t characteristics\_count

  - GATT\_CHARACTERISTIC\_T characteristics \[MAX\_SUPPORT\_SERVICE\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discover
characteristics command.

### Field Documentation

#### uint8\_t \_NCP\_DISC\_CHRC\_RP::characteristics\_count

> characteristic count

#### GATT\_CHARACTERISTIC\_T \_NCP\_DISC\_CHRC\_RP::characteristics\[MAX\_SUPPORT\_SERVICE\]

> characteristic information

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DISC\_CHRC\_UUID\_CMD Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure is used for NCP Bluetooth LE discover characteristic
UUID.

### Field Documentation

#### uint8\_t \_NCP\_DISC\_CHRC\_UUID\_CMD::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_DISC\_CHRC\_UUID\_CMD::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_DISC\_CHRC\_UUID\_CMD::start\_handle

> attribute start handle

#### uint16\_t \_NCP\_DISC\_CHRC\_UUID\_CMD::end\_handle

> attribute end handle

#### uint8\_t \_NCP\_DISC\_CHRC\_UUID\_CMD::uuid\_length

> UUID length

#### uint8\_t \_NCP\_DISC\_CHRC\_UUID\_CMD::uuid\[SERVER\_MAX\_UUID\_LEN\]

> UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DISC\_PRIM\_RP Struct Reference

### Data Fields

  - uint8\_t services\_count

  - GATT\_SERVICE\_T services \[MAX\_SUPPORT\_SERVICE\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discover
primary service command.

### Field Documentation

#### uint8\_t \_NCP\_DISC\_PRIM\_RP::services\_count

> service count

#### GATT\_SERVICE\_T \_NCP\_DISC\_PRIM\_RP::services\[MAX\_SUPPORT\_SERVICE\]

> service information

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_DISC\_PRIM\_UUID\_CMD Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure is used for NCP Bluetooth LE discover primary service
UUID.

### Field Documentation

#### uint8\_t \_NCP\_DISC\_PRIM\_UUID\_CMD::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_DISC\_PRIM\_UUID\_CMD::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t \_NCP\_DISC\_PRIM\_UUID\_CMD::uuid\_length

> UUID length

#### uint8\_t \_NCP\_DISC\_PRIM\_UUID\_CMD::uuid\[SERVER\_MAX\_UUID\_LEN\]

> UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_EVT\_CSI\_DATA Struct Reference

### Data Fields

  - uint16\_t Len

  - uint16\_t CSI\_Sign

  - uint32\_t CSI\_HeaderID

  - uint16\_t PKT\_info

  - uint16\_t FCF

  - uint64\_t TSF

  - uint8\_t Dst\_MAC \[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

  - uint8\_t Src\_MAC \[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

  - uint8\_t Rx\_RSSI\_A

  - uint8\_t Rx\_RSSI\_B

  - uint8\_t Rx\_NF\_A

  - uint8\_t Rx\_NF\_B

  - uint8\_t Rx\_SINR

  - uint8\_t channel

  - uint16\_t chip\_id

  - uint32\_t rsvd

  - uint32\_t CSI\_Data\_Length

  - uint8\_t CSI\_Data \[0\]

### Detailed Description

This structure is used for CSI (channel state information) data
configuration.

### Field Documentation

#### uint16\_t \_NCP\_EVT\_CSI\_DATA::Len

> Length in dwords, including header.

#### uint16\_t \_NCP\_EVT\_CSI\_DATA::CSI\_Sign

> CSI (channel state information) signature. 0xABCD fixed.

#### uint32\_t \_NCP\_EVT\_CSI\_DATA::CSI\_HeaderID

> User defined header ID.

#### uint16\_t \_NCP\_EVT\_CSI\_DATA::PKT\_info

> Packet info field.

#### uint16\_t \_NCP\_EVT\_CSI\_DATA::FCF

> Frame control field for the received packet.

#### uint64\_t \_NCP\_EVT\_CSI\_DATA::TSF

> Timestamp when packet received

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Dst\_MAC\[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

> Destination MAC address of received packet.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Src\_MAC\[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

> Source MAC address of received packet.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Rx\_RSSI\_A

> RX RSSI for antenna A.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Rx\_RSSI\_B

> RX RSSI for antenna B.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Rx\_NF\_A

> Noise floor for antenna A.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Rx\_NF\_B

> Noise floor for antenna B.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::Rx\_SINR

> RX signal strength above noise floor.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::channel

> Channel.

#### uint16\_t \_NCP\_EVT\_CSI\_DATA::chip\_id

> User defined chip ID.

#### uint32\_t \_NCP\_EVT\_CSI\_DATA::rsvd

> Reserved.

#### uint32\_t \_NCP\_EVT\_CSI\_DATA::CSI\_Data\_Length

> CSI data length in DWORDs.

#### uint8\_t \_NCP\_EVT\_CSI\_DATA::CSI\_Data\[0\]

> Start of CSI data.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_EVT\_MDNS\_RESOLVE Struct Reference

### Data Fields

  - uint8\_t ip\_type

  - union {

  -   uint32\_t **ip6\_addr** \[4\]

  -   uint32\_t **ip4\_addr**

  - } u\_addr

### Detailed Description

This structure is for MDNS domain event.

### Field Documentation

#### uint8\_t \_NCP\_EVT\_MDNS\_RESOLVE::ip\_type

> IP type,
> 
> 0: IPv4,
> 
> 1: IPv6.

#### union { ... } \_NCP\_EVT\_MDNS\_RESOLVE::u\_addr

> IPv4/IPv6 address.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_EVT\_MDNS\_RESULT Struct Reference

### Data Fields

  - uint32\_t ttl

  - uint32\_t tlv\_buf\_len

  - uint8\_t tlv\_buf \[1\]

### Detailed Description

This structure is used to store MDNS query results.

### Field Documentation

#### uint32\_t \_NCP\_EVT\_MDNS\_RESULT::ttl

> Time to live.

#### uint32\_t \_NCP\_EVT\_MDNS\_RESULT::tlv\_buf\_len

> Length of TLVs sent in command starting at buffer.

#### uint8\_t \_NCP\_EVT\_MDNS\_RESULT::tlv\_buf\[1\]

> Payload buffer of MDNS (multicast DNS) query command.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_GATT\_READ\_CMD Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t addr \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

### Detailed Description

This structure is used for NCP Bluetooth LE read attribute value.

### Field Documentation

#### uint8\_t \_NCP\_GATT\_READ\_CMD::type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_GATT\_READ\_CMD::addr\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_GATT\_READ\_CMD::handle

> Attribute handle to read with Characteristic Value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_IDENTITY\_RESOLVED\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t identity\_address\_type

  - uint8\_t identity\_address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the value of the Bluetooth LE identity resolved
event which indicates that the remote host Identity Address has been
resolved.

### Field Documentation

#### uint8\_t \_NCP\_IDENTITY\_RESOLVED\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_IDENTITY\_RESOLVED\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t \_NCP\_IDENTITY\_RESOLVED\_EV::identity\_address\_type

> Bluetooth LE identity address type
> 
> 2: public identity address
> 
> 3: random identity address

#### uint8\_t \_NCP\_IDENTITY\_RESOLVED\_EV::identity\_address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE identity address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_CONNECT\_CMD Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

  - uint8\_t sec

  - uint8\_t sec\_flag

### Detailed Description

This structure is used for NCP Bluetooth LE L2CAP connection.

### Field Documentation

#### uint8\_t \_NCP\_L2CAP\_CONNECT\_CMD::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_L2CAP\_CONNECT\_CMD::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_L2CAP\_CONNECT\_CMD::psm

> PSM (Protocol/Service Multiplexer) value

#### uint8\_t \_NCP\_L2CAP\_CONNECT\_CMD::sec

> connection security level
> 
> 0x01: No encryption and no authentication
> 
> 0x02: Encryption and no authentication (no MITM)
> 
> 0x03: Encryption and authentication (MITM)

#### uint8\_t \_NCP\_L2CAP\_CONNECT\_CMD::sec\_flag

> Option to use customized security level or to use system default
> security level
> 
> 0: to use system default security level
> 
> 1: to use customized security level

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_CONNECT\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

### Detailed Description

This structure contains the value of the Bluetooth LE L2CAP (Logical
Link Control and Adaptation Layer Protocol) connection complete event
which indicates an L2CAP connection is established.

### Field Documentation

#### uint8\_t \_NCP\_L2CAP\_CONNECT\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_L2CAP\_CONNECT\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_L2CAP\_CONNECT\_EV::psm

> PSM (Protocol/Service Multiplexer) value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_DISCONNECT\_CMD Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for NCP Bluetooth LE L2CAP disconnection.

### Field Documentation

#### uint8\_t \_NCP\_L2CAP\_DISCONNECT\_CMD::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_L2CAP\_DISCONNECT\_CMD::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_DISCONNECT\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

### Detailed Description

This structure contains the value of the Bluetooth LE L2CAP (Logical
Link Control and Adaptation Layer Protocol) disconnection complete event
which indicates an L2CAP connection is terminated.

### Field Documentation

#### uint8\_t \_NCP\_L2CAP\_DISCONNECT\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_L2CAP\_DISCONNECT\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_L2CAP\_DISCONNECT\_EV::psm

> PSM (Protocol/Service Multiplexer) value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_METRICS\_CMD Struct Reference

### Data Fields

  - bool metrics\_flag

### Detailed Description

This structure is used for NCP Bluetooth LE L2CAP set metrics.

### Field Documentation

#### bool \_NCP\_L2CAP\_METRICS\_CMD::metrics\_flag

> option to enable calculating metrics of l2cap rate
> 
> 0: disable calculating metrics of l2cap rate
> 
> 1: enable calculating metrics of l2cap rate

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_RECEIVE\_CMD Struct Reference

### Data Fields

  - uint32\_t l2cap\_recv\_delay\_ms

### Detailed Description

This structure is used for NCP Bluetooth LE L2CAP receive data.

### Field Documentation

#### uint32\_t \_NCP\_L2CAP\_RECEIVE\_CMD::l2cap\_recv\_delay\_ms

> receive delay time in millisecond

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_RECEIVE\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

  - uint8\_t len

  - uint8\_t data \[256\]

### Detailed Description

This structure contains the value of the Bluetooth LE L2CAP (Logical
Link Control and Adaptation Layer Protocol) receive packet event which
notifies the receive data over L2CAP channel to the client.

### Field Documentation

#### uint8\_t \_NCP\_L2CAP\_RECEIVE\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_L2CAP\_RECEIVE\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_L2CAP\_RECEIVE\_EV::psm

> PSM (Protocol/Service Multiplexer) value

#### uint8\_t \_NCP\_L2CAP\_RECEIVE\_EV::len

> receive data length

#### uint8\_t \_NCP\_L2CAP\_RECEIVE\_EV::data\[256\]

> receive data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_REGISTER\_CMD Struct Reference

### Data Fields

  - uint16\_t psm

  - uint8\_t sec\_level

  - uint8\_t sec\_flag

  - uint8\_t policy

  - uint8\_t policy\_flag

### Detailed Description

This structure is used for NCP Bluetooth LE L2CAP register PSM
(Protocol/Service Multiplexer).

### Field Documentation

#### uint16\_t \_NCP\_L2CAP\_REGISTER\_CMD::psm

> PSM (Protocol/Service Multiplexer) value

#### uint8\_t \_NCP\_L2CAP\_REGISTER\_CMD::sec\_level

> connection security level
> 
> 0x01: No encryption and no authentication
> 
> 0x02: Encryption and no authentication (no MITM)
> 
> 0x03: Encryption and authentication (MITM)

#### uint8\_t \_NCP\_L2CAP\_REGISTER\_CMD::sec\_flag

> Option to use custom security level or to use system default security
> level
> 
> 0: to use system default security level
> 
> 1: to use customized security level

#### uint8\_t \_NCP\_L2CAP\_REGISTER\_CMD::policy

> allowlist

#### uint8\_t \_NCP\_L2CAP\_REGISTER\_CMD::policy\_flag

> option to use allowlist
> 
> 0: not use allowlist
> 
> 1: use allowlist

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_L2CAP\_SEND\_CMD Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t times

### Detailed Description

This structure is used for NCP Bluetooth LE L2CAP send data.

### Field Documentation

#### uint8\_t \_NCP\_L2CAP\_SEND\_CMD::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_L2CAP\_SEND\_CMD::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t \_NCP\_L2CAP\_SEND\_CMD::times

> the number of times a loop is sent

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_NCS\_INFO\_RP Struct Reference

### Data Fields

  - char ssid \[32\]

  - uint8\_t ssid\_len

  - char pswd \[65\]

  - uint8\_t pswd\_len

  - char secu \[10\]

  - uint8\_t secu\_len

### Detailed Description

This structure is returned as an output from the Bluetooth LE commission
service command.

### Field Documentation

#### char \_NCP\_NCS\_INFO\_RP::ssid\[32\]

> ssid value

#### uint8\_t \_NCP\_NCS\_INFO\_RP::ssid\_len

> ssid length

#### char \_NCP\_NCS\_INFO\_RP::pswd\[65\]

> password value

#### uint8\_t \_NCP\_NCS\_INFO\_RP::pswd\_len

> password length

#### char \_NCP\_NCS\_INFO\_RP::secu\[10\]

> security value

#### uint8\_t \_NCP\_NCS\_INFO\_RP::secu\_len

> security length

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_NOTIFICATION\_EV Struct Reference

### Data Fields

  - uint8\_t svc\_id

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t type

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[MAX\_ATTRIBUTE\_VALUE\_LEN\]

### Detailed Description

This structure contains the value of the Bluetooth LE GATT notification
event which notifies characteristic values to the client.

### Field Documentation

#### uint8\_t \_NCP\_NOTIFICATION\_EV::svc\_id

> service id

#### uint8\_t \_NCP\_NOTIFICATION\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_NOTIFICATION\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t \_NCP\_NOTIFICATION\_EV::type

> notification type

#### uint16\_t \_NCP\_NOTIFICATION\_EV::handle

> notification handle

#### uint16\_t \_NCP\_NOTIFICATION\_EV::data\_length

> notification data length

#### uint8\_t \_NCP\_NOTIFICATION\_EV::data\[MAX\_ATTRIBUTE\_VALUE\_LEN\]

> notification data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_PASSKEY\_DISPLAY\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint32\_t passkey

### Detailed Description

This structure contains the value of the Bluetooth LE user passkey
notification event which is used to provide a passkey for the Host to
display to the user.

### Field Documentation

#### uint8\_t \_NCP\_PASSKEY\_DISPLAY\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_PASSKEY\_DISPLAY\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint32\_t \_NCP\_PASSKEY\_DISPLAY\_EV::passkey

> device passkey number

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_REGISTER\_SERVICE Struct Reference

### Data Fields

  - uint8\_t svc\_length

  - uint8\_t service \[MAX\_SUPPORT\_SERVICE\]

### Detailed Description

This structure is used for NCP Bluetooth LE register service.

### Field Documentation

#### uint8\_t \_NCP\_REGISTER\_SERVICE::svc\_length

> service length

#### uint8\_t \_NCP\_REGISTER\_SERVICE::service\[MAX\_SUPPORT\_SERVICE\]

> service id
> 
> 1: Peripheral\_HTS
> 
> 2: Peripheral\_HRS
> 
> 3: BAS
> 
> 4: Central\_HTS
> 
> 5: Central\_HRS

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_SEC\_LEVEL\_CHANGED\_EV Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t sec\_level

### Detailed Description

This structure contains the value of the Bluetooth LE security level
changed event which is sent when the connection security level has
changed.

### Field Documentation

#### uint8\_t \_NCP\_SEC\_LEVEL\_CHANGED\_EV::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t \_NCP\_SEC\_LEVEL\_CHANGED\_EV::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t \_NCP\_SEC\_LEVEL\_CHANGED\_EV::sec\_level

> connection security level
> 
> 0x01: No encryption and no authentication
> 
> 0x02: Encryption and no authentication (no MITM)
> 
> 0x03: Encryption and authentication (MITM)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_SET\_VALUE\_CMD Struct Reference

### Data Fields

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

  - uint16\_t len

  - uint8\_t value \[512\]

### Detailed Description

This structure is used for NCP Bluetooth LE set attribute value.

### Field Documentation

#### uint8\_t \_NCP\_SET\_VALUE\_CMD::uuid\_length

> UUID length

#### uint8\_t \_NCP\_SET\_VALUE\_CMD::uuid\[SERVER\_MAX\_UUID\_LEN\]

> UUID value

#### uint16\_t \_NCP\_SET\_VALUE\_CMD::len

> data length

#### uint8\_t \_NCP\_SET\_VALUE\_CMD::value\[512\]

> data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_START\_ADV\_RP Struct Reference

### Data Fields

  - uint32\_t adv\_setting

### Detailed Description

This structure contains the command response values of the Bluetooth LE
start advertising command.

### Field Documentation

#### uint32\_t \_NCP\_START\_ADV\_RP::adv\_setting

> device current setting
> 
> bit0: GAP\_SETTINGS\_POWERED
> 
> bit1: GAP\_SETTINGS\_CONNECTABLE
> 
> bit2: GAP\_SETTINGS\_FAST\_CONNECTABLE
> 
> bit3: GAP\_SETTINGS\_DISCOVERABLE
> 
> bit4: GAP\_SETTINGS\_BONDABLE
> 
> bit5: GAP\_SETTINGS\_LINK\_SEC\_3
> 
> bit6: GAP\_SETTINGS\_SSP
> 
> bit7: GAP\_SETTINGS\_BREDR
> 
> bit8: GAP\_SETTINGS\_HS
> 
> bit9: GAP\_SETTINGS\_LE
> 
> bit10: GAP\_SETTINGS\_ADVERTISING
> 
> bit11: GAP\_SETTINGS\_SC
> 
> bit12: GAP\_SETTINGS\_DEBUG\_KEYS
> 
> bit13: GAP\_SETTINGS\_PRIVACY
> 
> bit14: GAP\_SETTINGS\_CONTROLLER\_CONFIG
> 
> bit15: GAP\_SETTINGS\_STATIC\_ADDRESS

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_NCP\_SUBSCRIPTION\_EV Struct Reference

### Data Fields

  - uint8\_t svc\_id

  - uint8\_t status

### Detailed Description

This structure contains the value of the Bluetooth LE subscription event
which indicates the service characteristics are subscribed

### Field Documentation

#### uint8\_t \_NCP\_SUBSCRIPTION\_EV::svc\_id

> service id

#### uint8\_t \_NCP\_SUBSCRIPTION\_EV::status

> subscription status

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_ncp\_wlan\_ipv4\_config Struct Reference

### Data Fields

  - uint32\_t addr\_type: 2

  - uint32\_t address

  - uint32\_t gw

  - uint32\_t netmask

  - uint32\_t dns1

  - uint32\_t dns2

### Detailed Description

This structure is used for IPv4 configuration.

### Field Documentation

#### uint32\_t \_ncp\_wlan\_ipv4\_config::addr\_type

> Set to ADDR\_TYPE\_DHCP to use DHCP to obtain the IP address or
> ADDR\_TYPE\_STATIC to use a static IP. In case of static IP address
> ip, gateway, netmask and dns members must be specified. When using
> DHCP, the IP, gateway, netmask and DNS are overwritten by the values
> obtained from the DHCP server. They should be zeroed out if not used.

#### uint32\_t \_ncp\_wlan\_ipv4\_config::address

> IP address in network order of the system.

#### uint32\_t \_ncp\_wlan\_ipv4\_config::gw

> Default gateway in network order of the system.

#### uint32\_t \_ncp\_wlan\_ipv4\_config::netmask

> Subnet mask in network order of the system.

#### uint32\_t \_ncp\_wlan\_ipv4\_config::dns1

> Primary dns server in network order of the system.

#### uint32\_t \_ncp\_wlan\_ipv4\_config::dns2

> Secondary DNS server in network order of the system.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_ncp\_wlan\_ipv6\_config Struct Reference

### Data Fields

  - unsigned address \[MLAN\_IPV6\_ADDR\_LEN\]

  - unsigned char addr\_type\_str \[IPV6\_MAX\_ADDR\_TYPE\_STR\_LEN\]

  - unsigned char addr\_state\_str \[IPV6\_MAX\_ADDR\_STATE\_STR\_LEN\]

### Detailed Description

This structure is used for IPv6 configuration.

### Field Documentation

#### unsigned \_ncp\_wlan\_ipv6\_config::address\[MLAN\_IPV6\_ADDR\_LEN\]

> System's IPv6 address in network order.

#### unsigned char \_ncp\_wlan\_ipv6\_config::addr\_type\_str\[IPV6\_MAX\_ADDR\_TYPE\_STR\_LEN\]

> Address type string: linklocal, site-local or global.

#### unsigned char \_ncp\_wlan\_ipv6\_config::addr\_state\_str\[IPV6\_MAX\_ADDR\_STATE\_STR\_LEN\]

> State string of IPv6 address (Tentative, Preferred, etc).

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_WLAN\_NET\_MONITOR\_PARA Struct Reference

### Data Fields

  - uint16\_t action

  - uint16\_t monitor\_activity

  - uint16\_t filter\_flags

  - uint8\_t radio\_type

  - uint8\_t chan\_number

  - uint8\_t filter\_num

  - uint8\_t mac\_addr
    \[MAX\_MONIT\_MAC\_FILTER\_NUM\]\[MLAN\_MAC\_ADDR\_LENGTH\]

### Detailed Description

This structure is used for network monitor configuration.

### Field Documentation

#### uint16\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::action

> Action,
> 
> add: add mac addr as filter element
> 
> delete: delete mac addr
> 
> clear: clear mac addr
> 
> dump: dump monitor parameter.

#### uint16\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::monitor\_activity

> Monitor activity enable flag, 1 enable, other disable.

#### uint16\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::filter\_flags

> network monitor filter flag.

#### uint8\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::radio\_type

> Channel scan parameter: radio type.

#### uint8\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::chan\_number

> Channel to monitor.

#### uint8\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::filter\_num

> mac num of filter.

#### uint8\_t \_NCP\_WLAN\_NET\_MONITOR\_PARA::mac\_addr\[MAX\_MONIT\_MAC\_FILTER\_NUM\]\[MLAN\_MAC\_ADDR\_LENGTH\]

> Source address of the packet to receive.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_ncp\_wlan\_network Struct Reference

### Data Fields

  - char name \[WLAN\_NETWORK\_NAME\_MAX\_LENGTH+1\]

  - char ssid \[IEEEtypes\_SSID\_SIZE+1\]

  - char bssid \[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

  - unsigned int channel

  - uint16\_t acs\_band

  - short rssi\_threshold

  - uint8\_t type

  - uint8\_t role

  - uint8\_t security\_type

  - uint8\_t enable\_11ax: 1

  - uint8\_t enable\_11ac: 1

  - uint8\_t enable\_11n: 1

  - NCP\_WLAN\_IPV6\_CONFIG ipv6 \[CONFIG\_MAX\_IPV6\_ADDRESSES\]

  - NCP\_WLAN\_IPV4\_CONFIG ipv4

  - uint8\_t is\_sta\_ipv4\_connected

  - char identity \[IDENTITY\_MAX\_LENGTH\]

  - unsigned ssid\_specific: 1

  - unsigned bssid\_specific: 1

  - unsigned channel\_specific: 1

  - unsigned security\_specific: 1

  - unsigned wps\_specific: 1

  - uint16\_t beacon\_period

  - uint8\_t dtim\_period

  - uint8\_t wlan\_capa

### Detailed Description

This structure is used to store a Wi-Fi network profile.

### Field Documentation

#### char \_ncp\_wlan\_network::name\[WLAN\_NETWORK\_NAME\_MAX\_LENGTH+1\]

> Name of this network profile.

#### char \_ncp\_wlan\_network::ssid\[IEEEtypes\_SSID\_SIZE+1\]

> Network SSID, represented as a C string of up to 32 characters in
> length. If this profile is used in the UAP mode, this field is used as
> the SSID of the network. If this profile is used in the station mode,
> this field is used to identify the network. Set the first byte of the
> SSID to NULL (a 0-length string) to use only the BSSID to find the
> network.

#### char \_ncp\_wlan\_network::bssid\[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

> Network BSSID, represented as a 6-byte array. If this profile is used
> in the UAP mode, this field is ignored. If this profile is used in the
> station mode, this field is used to identify the network. Set all 6
> bytes to 0 to use any BSSID, in which case only the SSID will be used
> to find the network.

#### unsigned int \_ncp\_wlan\_network::channel

> Channel for this network. If this profile is used in UAP mode, this
> field specifies the channel to start the UAP interface on. Set this to
> 0 for auto channel selection. If this profile is used in the station
> mode, this constrains the channel on which the network to connect
> should be present. Set this to 0 to allow the network to be found on
> any channel.

#### uint16\_t \_ncp\_wlan\_network::acs\_band

> If set channel to 0, enable auto channel select.

#### short \_ncp\_wlan\_network::rssi\_threshold

> Rssi threshold is used to scan with RSSI filter.

#### uint8\_t \_ncp\_wlan\_network::type

> BSS type.

#### uint8\_t \_ncp\_wlan\_network::role

> Network role type, set this to specify what type of Wi-Fi network mode
> to use. This can either be WLAN\_BSS\_ROLE\_STA for use in the station
> mode, or it can be WLAN\_BSS\_ROLE\_UAP for use in the UAP mode.

#### uint8\_t \_ncp\_wlan\_network::security\_type

> Network's security type. Use specified by enum wlan\_security\_type.

#### uint8\_t \_ncp\_wlan\_network::enable\_11ax

> Enable 802.11ax flag,
> 
> 1: enable;
> 
> 0: disable.

#### uint8\_t \_ncp\_wlan\_network::enable\_11ac

> Enable 802.11ac flag,
> 
> 1: enable;
> 
> 0: disable.

#### uint8\_t \_ncp\_wlan\_network::enable\_11n

> Enable 802.11n flag,
> 
> 1: enable;
> 
> 0: disable.

#### NCP\_WLAN\_IPV6\_CONFIG \_ncp\_wlan\_network::ipv6\[CONFIG\_MAX\_IPV6\_ADDRESSES\]

> Configuration of network is IPv6 address.

#### NCP\_WLAN\_IPV4\_CONFIG \_ncp\_wlan\_network::ipv4

> Configuration of network is IPv4 address.

#### uint8\_t \_ncp\_wlan\_network::is\_sta\_ipv4\_connected

> STA connected AP with IPV4 flag, TRUE means STA is connected to AP,
> FALSE means STA is disconnected to AP.

#### char \_ncp\_wlan\_network::identity\[IDENTITY\_MAX\_LENGTH\]

> Identity string.

#### unsigned \_ncp\_wlan\_network::ssid\_specific

> If set to 1, the ssid field contains the specific SSID for this
> network.

#### unsigned \_ncp\_wlan\_network::bssid\_specific

> If set to 1, the BSSID (basic service set ID) field contains the
> specific BSSID for this network.

#### unsigned \_ncp\_wlan\_network::channel\_specific

> If set to 1, the channel field contains the specific channel for this
> network.

#### unsigned \_ncp\_wlan\_network::security\_specific

> Set to 0, any security that matches is used.

#### unsigned \_ncp\_wlan\_network::wps\_specific

> This indicates this network is used as an internal network for WPS.

#### uint16\_t \_ncp\_wlan\_network::beacon\_period

> Beacon period of associated BSS (basic service set).

#### uint8\_t \_ncp\_wlan\_network::dtim\_period

> DTIM period of associated BSS.

#### uint8\_t \_ncp\_wlan\_network::wlan\_capa

> Wi-Fi network capability.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCP\_WLAN\_RSSI\_INFO\_T Struct Reference

### Data Fields

  - int16\_t data\_rssi\_last

  - int16\_t data\_nf\_last

  - int16\_t data\_rssi\_avg

  - int16\_t data\_nf\_avg

  - int16\_t bcn\_snr\_last

  - int16\_t bcn\_snr\_avg

  - int16\_t data\_snr\_last

  - int16\_t data\_snr\_avg

  - int16\_t bcn\_rssi\_last

  - int16\_t bcn\_nf\_last

  - int16\_t bcn\_rssi\_avg

  - int16\_t bcn\_nf\_avg

### Detailed Description

This structure is used for RSSI (received signal strength indicator)
information configuration.

### Field Documentation

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::data\_rssi\_last

> Value of last RSSI.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::data\_nf\_last

> Value of last NF (noise floor).

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::data\_rssi\_avg

> Average value of RSSI.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::data\_nf\_avg

> Average value of NF (noise floor).

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::bcn\_snr\_last

> SNR (signal noise ratio) of last beacon.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::bcn\_snr\_avg

> Average SNR (signal noise ratio) of beacon.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::data\_snr\_last

> SNR (signal noise ratio) of last data.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::data\_snr\_avg

> Average SNR (signal noise ratio) of data.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::bcn\_rssi\_last

> RSSI of last beacon.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::bcn\_nf\_last

> NF (noise floor) of last beacon.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::bcn\_rssi\_avg

> Average RSSI of beacon.

#### int16\_t \_NCP\_WLAN\_RSSI\_INFO\_T::bcn\_nf\_avg

> Average NF of beacon.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_ncp\_wlan\_scan\_result Struct Reference

### Data Fields

  - char ssid \[IEEEtypes\_SSID\_SIZE+1\]

  - unsigned int ssid\_len

  - char bssid \[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

  - unsigned int channel

  - unsigned dot11n: 1

  - unsigned dot11ac: 1

  - unsigned dot11ax: 1

  - unsigned **wmm**: 1

  - unsigned wps: 1

  - unsigned int wps\_session

  - unsigned wpa2\_entp: 1

  - unsigned wep: 1

  - unsigned wpa: 1

  - unsigned wpa2: 1

  - unsigned wpa2\_sha256: 1

  - unsigned wpa3\_sae: 1

  - unsigned char rssi

  - char trans\_ssid \[IEEEtypes\_SSID\_SIZE+1\]

  - unsigned int trans\_ssid\_len

  - char trans\_bssid \[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

  - uint16\_t beacon\_period

  - uint8\_t dtim\_period

  - uint8\_t ap\_mfpc

  - uint8\_t ap\_mfpr

  - bool neighbor\_report\_supported

  - bool bss\_transition\_supported

### Detailed Description

This structure is used to store the information of one scanned AP

### Field Documentation

#### char \_ncp\_wlan\_scan\_result::ssid\[IEEEtypes\_SSID\_SIZE+1\]

> Network SSID (service set ID) , represented as a NULL-terminated C
> string of 0 to 32 characters. If the network has a hidden SSID, this
> will be the empty string.

#### unsigned int \_ncp\_wlan\_scan\_result::ssid\_len

> SSID length.

#### char \_ncp\_wlan\_scan\_result::bssid\[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

> Network BSSID (basic servivce set ID), represented as a 6-byte array.

#### unsigned int \_ncp\_wlan\_scan\_result::channel

> Network channel.

#### unsigned \_ncp\_wlan\_scan\_result::dot11n

> Network supports 802.11n. This is set to 0 if the network does not
> support 802.11n or if the system does not have 802.11n support
> enabled.

#### unsigned \_ncp\_wlan\_scan\_result::dot11ac

> Network supports 802.11ac. This is set to 0 if the network does not
> support 802.11ac or if the system does not have 802.11ac support
> enabled.

#### unsigned \_ncp\_wlan\_scan\_result::dot11ax

> Network supports 802.11ax. This is set to 0 if the network does not
> support 802.11ax or if the system does not have 802.11ax support
> enabled.

#### unsigned \_ncp\_wlan\_scan\_result::wps

> Network supports WPS (Wi-Fi protected setup). This is set to 0 if the
> network does not support WPS or if the system does not have WPS
> support enabled.

#### unsigned int \_ncp\_wlan\_scan\_result::wps\_session

> WPS type PBC (push button configuration)/PIN

#### unsigned \_ncp\_wlan\_scan\_result::wpa2\_entp

> WPA2 enterprise security

#### unsigned \_ncp\_wlan\_scan\_result::wep

> Network uses WEP security.

#### unsigned \_ncp\_wlan\_scan\_result::wpa

> Network uses WPA security.

#### unsigned \_ncp\_wlan\_scan\_result::wpa2

> Network uses WPA2 security.

#### unsigned \_ncp\_wlan\_scan\_result::wpa2\_sha256

> The network uses WPA2 SHA256 security

#### unsigned \_ncp\_wlan\_scan\_result::wpa3\_sae

> Network uses WPA3 SAE (simultaneous authentication of equals)
> security.

#### unsigned char \_ncp\_wlan\_scan\_result::rssi

> Signal strength of the beacon.

#### char \_ncp\_wlan\_scan\_result::trans\_ssid\[IEEEtypes\_SSID\_SIZE+1\]

> Network SSID (service set ID), represented as a NULL-terminated C
> string of 0 to 32 characters. If the network has a hidden SSID, this
> will be the empty string.

#### unsigned int \_ncp\_wlan\_scan\_result::trans\_ssid\_len

> SSID length

#### char \_ncp\_wlan\_scan\_result::trans\_bssid\[IEEEtypes\_MAC\_ADDRESS\_SIZE\]

> Network BSSID (basic service set ID), represented as a 6-byte array.

#### uint16\_t \_ncp\_wlan\_scan\_result::beacon\_period

> Beacon period.

#### uint8\_t \_ncp\_wlan\_scan\_result::dtim\_period

> DTIM (delivery traffic indication map) period.

#### uint8\_t \_ncp\_wlan\_scan\_result::ap\_mfpc

> MFPC (Management Frame Protection Capable) bit of AP (Access Point)

#### uint8\_t \_ncp\_wlan\_scan\_result::ap\_mfpr

> MFPR (Management Frame Protection Required) bit of AP (Access Point)

#### bool \_ncp\_wlan\_scan\_result::neighbor\_report\_supported

> Neighbor report support

#### bool \_ncp\_wlan\_scan\_result::bss\_transition\_supported

> bss transition support

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCPCmd\_DS\_COMMAND Struct Reference

### Data Fields

  - NCP\_COMMAND header

  - union {

  -   NCP\_CMD\_SCAN\_NETWORK\_INFO scan\_network\_info

  -   NCP\_CMD\_FW\_VERSION fw\_version

  -   NCP\_CMD\_MAC\_ADDRESS mac\_addr

  -   NCP\_CMD\_GET\_MAC\_ADDRESS get\_mac\_addr

  -   NCP\_CMD\_CONNECT\_STAT conn\_stat

  -   NCP\_CMD\_NETWORK\_INFO network\_info

  -   NCP\_CMD\_NETWORK\_ADDRESS network\_address

  -   NCP\_CMD\_NET\_MONITOR **monitor\_cfg**

  -   NCP\_CMD\_NETWORK\_ADD network\_add

  -   NCP\_CMD\_NETWORK\_START network\_start

  -   NCP\_CMD\_NETWORK\_UAP\_STA\_LIST uap\_sta\_list

  -   NCP\_CMD\_CSI csi\_cfg

  -   NCP\_CMD\_11K\_CFG wlan\_11k\_cfg

  -   NCP\_CMD\_NEIGHBOR\_REQ neighbor\_req

  -   NCP\_CMD\_MBO\_ENABLE wlan\_mbo\_enable

  -   NCP\_CMD\_MBO\_NONPREFER\_CH mbo\_nonprefer\_ch

  -   NCP\_CMD\_MBO\_SET\_CELL\_CAPA wlan\_mbo\_set\_cell\_capa

  -   NCP\_CMD\_MBO\_SET\_OCE wlan\_mbo\_set\_oce

  -   NCP\_CMD\_RSSI signal\_rssi

  -   NCP\_CMD\_ROAMING roaming\_cfg

  -   NCP\_CMD\_CLIENT\_CNT max\_client\_count

  -   NCP\_CMD\_ANTENNA\_CFG antenna\_cfg

  -   NCP\_CMD\_WPS\_GEN\_PIN wps\_gen\_pin\_info

  -   NCP\_CMD\_WPS\_PIN wps\_pin\_cfg

  -   NCP\_CMD\_11AX\_CFG\_INFO he\_cfg

  -   NCP\_CMD\_BTWT\_CFG\_INFO btwt\_cfg

  -   NCP\_CMD\_TWT\_SETUP\_CFG twt\_setup

  -   NCP\_CMD\_TWT\_TEARDOWN\_CFG twt\_teardown

  -   NCP\_CMD\_11D\_ENABLE\_CFG wlan\_11d\_cfg

  -   NCP\_CMD\_SOCKET\_OPEN\_CFG wlan\_socket\_open

  -   NCP\_CMD\_SOCKET\_CON\_CFG wlan\_socket\_con

  -   NCP\_CMD\_SOCKET\_BIND\_CFG wlan\_socket\_bind

  -   NCP\_CMD\_SOCKET\_CLOSE\_CFG wlan\_socket\_close

  -   NCP\_CMD\_SOCKET\_LISTEN\_CFG wlan\_socket\_listen

  -   NCP\_CMD\_SOCKET\_ACCEPT\_CFG wlan\_socket\_accept

  -   NCP\_CMD\_SOCKET\_SEND\_CFG wlan\_socket\_send

  -   NCP\_CMD\_SOCKET\_SENDTO\_CFG wlan\_socket\_sendto

  -   NCP\_CMD\_SOCKET\_RECEIVE\_CFG wlan\_socket\_receive

  -   NCP\_CMD\_SOCKET\_RECVFROM\_CFG wlan\_socket\_recvfrom

  -   NCP\_CMD\_HTTP\_CON\_CFG wlan\_http\_connect

  -   NCP\_CMD\_UAP\_PROV\_SET\_UAPCFG prov\_set\_uapcfg

  -   NCP\_CMD\_HTTP\_DISCON\_CFG wlan\_http\_disconnect

  -   NCP\_CMD\_HTTP\_SETH\_CFG wlan\_http\_seth

  -   NCP\_CMD\_HTTP\_UNSETH\_CFG wlan\_http\_unseth

  -   NCP\_CMD\_HTTP\_REQ\_CFG wlan\_http\_req

  -   NCP\_CMD\_HTTP\_REQ\_RESP\_CFG wlan\_http\_req\_resp

  -   NCP\_CMD\_HTTP\_RECV\_CFG wlan\_http\_recv

  -   NCP\_CMD\_HTTP\_UPG\_CFG wlan\_http\_upg

  -   NCP\_CMD\_WEBSOCKET\_SEND\_CFG wlan\_websocket\_send

  -   NCP\_CMD\_WEBSOCKET\_RECV\_CFG wlan\_websocket\_recv

  -   NCP\_CMD\_POWERMGMT\_MEF mef\_config

  -   NCP\_CMD\_DEEP\_SLEEP\_PS wlan\_deep\_sleep\_ps

  -   NCP\_CMD\_IEEE\_PS wlan\_ieee\_ps

  -   NCP\_CMD\_POWERMGMT\_UAPSD uapsd\_cfg

  -   NCP\_CMD\_POWERMGMT\_QOSINFO qosinfo\_cfg

  -   NCP\_CMD\_POWERMGMT\_SLEEP\_PERIOD sleep\_period\_cfg

  -   NCP\_CMD\_POWERMGMT\_WOWLAN\_CFG wowlan\_config

  -   NCP\_CMD\_POWERMGMT\_SUSPEND suspend\_config

  -   WLAN\_RESET\_data reset\_config

  -   NCP\_CMD\_TWT\_REPORT twt\_report

  -   NCP\_CMD\_REGION\_CODE\_CFG region\_cfg

  -   NCP\_CMD\_ED\_MAC ed\_mac\_mode

  -   NCP\_CMD\_RF\_TX\_ANTENNA rf\_tx\_antenna

  -   NCP\_CMD\_RF\_RX\_ANTENNA rf\_rx\_antenna

  -   NCP\_CMD\_RF\_BAND rf\_band

  -   NCP\_CMD\_RF\_BANDWIDTH rf\_bandwidth

  -   NCP\_CMD\_RF\_CHANNEL rf\_channel

  -   NCP\_CMD\_RF\_RADIO\_MODE rf\_radio\_mode

  -   NCP\_CMD\_RF\_TX\_POWER rf\_tx\_power

  -   NCP\_CMD\_RF\_TX\_CONT\_MODE rf\_tx\_cont\_mode

  -   NCP\_CMD\_RF\_TX\_FRAME rf\_tx\_frame

  -   NCP\_CMD\_RF\_PER rf\_per

  -   NCP\_CMD\_EU\_CRYPRTO eu\_crypto

  -   NCP\_CMD\_REGISTER\_ACCESS register\_access

  -   NCP\_CMD\_MEM\_STAT mem\_stat

  -   NCP\_CMD\_DATE\_TIME\_CFG date\_time

  -   NCP\_CMD\_TEMPERATURE temperature

  -   NCP\_CMD\_WLAN\_CONN wlan\_connect

  -   NCP\_CMD\_MDNS\_QUERY mdns\_query

  -   NCP\_EVT\_MDNS\_RESULT mdns\_result

  -   NCP\_EVT\_MDNS\_RESOLVE mdns\_resolve

  -   NCP\_EVT\_CSI\_DATA csi\_data

  -   NCP\_CMD\_NETWORK\_LIST network\_list

  -   NCP\_CMD\_NETWORK\_REMOVE network\_remove

  -   NCP\_CMD\_GET\_CURRENT\_NETWORK current\_network

  - } params

### Detailed Description

NCP command structure.

### Field Documentation

#### NCP\_COMMAND \_NCPCmd\_DS\_COMMAND::header

> Command header.

#### NCP\_CMD\_SCAN\_NETWORK\_INFO \_NCPCmd\_DS\_COMMAND::scan\_network\_info

> Scan result.

#### NCP\_CMD\_FW\_VERSION \_NCPCmd\_DS\_COMMAND::fw\_version

> Driver and firmware version

#### NCP\_CMD\_MAC\_ADDRESS \_NCPCmd\_DS\_COMMAND::mac\_addr

> MAC address

#### NCP\_CMD\_GET\_MAC\_ADDRESS \_NCPCmd\_DS\_COMMAND::get\_mac\_addr

> Get MAC address

#### NCP\_CMD\_CONNECT\_STAT \_NCPCmd\_DS\_COMMAND::conn\_stat

> wlan connection state

#### NCP\_CMD\_NETWORK\_INFO \_NCPCmd\_DS\_COMMAND::network\_info

> wlan network info

#### NCP\_CMD\_NETWORK\_ADDRESS \_NCPCmd\_DS\_COMMAND::network\_address

> wlan network address

#### NCP\_CMD\_NETWORK\_ADD \_NCPCmd\_DS\_COMMAND::network\_add

> wlan add network

#### NCP\_CMD\_NETWORK\_START \_NCPCmd\_DS\_COMMAND::network\_start

> wlan start network

#### NCP\_CMD\_NETWORK\_UAP\_STA\_LIST \_NCPCmd\_DS\_COMMAND::uap\_sta\_list

> wlan UAP station list

#### NCP\_CMD\_CSI \_NCPCmd\_DS\_COMMAND::csi\_cfg

> CSI configuration

#### NCP\_CMD\_11K\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_11k\_cfg

> 802.11k enable configuration

#### NCP\_CMD\_NEIGHBOR\_REQ \_NCPCmd\_DS\_COMMAND::neighbor\_req

> neighbor report request

#### NCP\_CMD\_MBO\_ENABLE \_NCPCmd\_DS\_COMMAND::wlan\_mbo\_enable

> MBO enable configure

#### NCP\_CMD\_MBO\_NONPREFER\_CH \_NCPCmd\_DS\_COMMAND::mbo\_nonprefer\_ch

> NCP MBO non prefer channel structure

#### NCP\_CMD\_MBO\_SET\_CELL\_CAPA \_NCPCmd\_DS\_COMMAND::wlan\_mbo\_set\_cell\_capa

> MBO cell capacity

#### NCP\_CMD\_MBO\_SET\_OCE \_NCPCmd\_DS\_COMMAND::wlan\_mbo\_set\_oce

> NCP MBO set OCE

#### NCP\_CMD\_RSSI \_NCPCmd\_DS\_COMMAND::signal\_rssi

> RSSI information

#### NCP\_CMD\_ROAMING \_NCPCmd\_DS\_COMMAND::roaming\_cfg

> Roaming configurations

#### NCP\_CMD\_CLIENT\_CNT \_NCPCmd\_DS\_COMMAND::max\_client\_count

> MAX client count

#### NCP\_CMD\_ANTENNA\_CFG \_NCPCmd\_DS\_COMMAND::antenna\_cfg

> Antenna config

#### NCP\_CMD\_WPS\_GEN\_PIN \_NCPCmd\_DS\_COMMAND::wps\_gen\_pin\_info

> Generated WPS pin code

#### NCP\_CMD\_WPS\_PIN \_NCPCmd\_DS\_COMMAND::wps\_pin\_cfg

> wps pin code

#### NCP\_CMD\_11AX\_CFG\_INFO \_NCPCmd\_DS\_COMMAND::he\_cfg

> 802.11ax configure information

#### NCP\_CMD\_BTWT\_CFG\_INFO \_NCPCmd\_DS\_COMMAND::btwt\_cfg

> TWT broadcast configuration information

#### NCP\_CMD\_TWT\_SETUP\_CFG \_NCPCmd\_DS\_COMMAND::twt\_setup

> TWT setup configuration

#### NCP\_CMD\_TWT\_TEARDOWN\_CFG \_NCPCmd\_DS\_COMMAND::twt\_teardown

> TWT teardown configuration

#### NCP\_CMD\_11D\_ENABLE\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_11d\_cfg

> 802.11d enable configuration

#### NCP\_CMD\_SOCKET\_OPEN\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_open

> socket open configuration

#### NCP\_CMD\_SOCKET\_CON\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_con

> socket connect configuration

#### NCP\_CMD\_SOCKET\_BIND\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_bind

> socket bind configuration

#### NCP\_CMD\_SOCKET\_CLOSE\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_close

> socket close configuration

#### NCP\_CMD\_SOCKET\_LISTEN\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_listen

> socket listen configuration

#### NCP\_CMD\_SOCKET\_ACCEPT\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_accept

> socket accept configuration

#### NCP\_CMD\_SOCKET\_SEND\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_send

> socket send configuration

#### NCP\_CMD\_SOCKET\_SENDTO\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_sendto

> socket sendto configuration

#### NCP\_CMD\_SOCKET\_RECEIVE\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_receive

> socket receive configuration

#### NCP\_CMD\_SOCKET\_RECVFROM\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_socket\_recvfrom

> socket recvfrom configuration

#### NCP\_CMD\_HTTP\_CON\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_connect

> http connect configuration

#### NCP\_CMD\_UAP\_PROV\_SET\_UAPCFG \_NCPCmd\_DS\_COMMAND::prov\_set\_uapcfg

> provisioning UAP configuration

#### NCP\_CMD\_HTTP\_DISCON\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_disconnect

> http disconnect configuration

#### NCP\_CMD\_HTTP\_SETH\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_seth

> http seth.

#### NCP\_CMD\_HTTP\_UNSETH\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_unseth

> http unseth.

#### NCP\_CMD\_HTTP\_REQ\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_req

> http request.

#### NCP\_CMD\_HTTP\_REQ\_RESP\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_req\_resp

> http request response

#### NCP\_CMD\_HTTP\_RECV\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_recv

> http receive configuration.

#### NCP\_CMD\_HTTP\_UPG\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_http\_upg

> Http upgrade

#### NCP\_CMD\_WEBSOCKET\_SEND\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_websocket\_send

> NCP web socket send configuration.

#### NCP\_CMD\_WEBSOCKET\_RECV\_CFG \_NCPCmd\_DS\_COMMAND::wlan\_websocket\_recv

> NCP web socket receive configuration.

#### NCP\_CMD\_POWERMGMT\_MEF \_NCPCmd\_DS\_COMMAND::mef\_config

> MEF configurations.

#### NCP\_CMD\_DEEP\_SLEEP\_PS \_NCPCmd\_DS\_COMMAND::wlan\_deep\_sleep\_ps

> Wi-Fi deep sleep power save mode.

#### NCP\_CMD\_IEEE\_PS \_NCPCmd\_DS\_COMMAND::wlan\_ieee\_ps

> Wi-Fi IEEE power save mode configure.

#### NCP\_CMD\_POWERMGMT\_UAPSD \_NCPCmd\_DS\_COMMAND::uapsd\_cfg

> NCP enable/disable UAPSD configuration

#### NCP\_CMD\_POWERMGMT\_QOSINFO \_NCPCmd\_DS\_COMMAND::qosinfo\_cfg

> NCP set/get WMM QoS configuration

#### NCP\_CMD\_POWERMGMT\_SLEEP\_PERIOD \_NCPCmd\_DS\_COMMAND::sleep\_period\_cfg

> NCP set/get sleep period

#### NCP\_CMD\_POWERMGMT\_WOWLAN\_CFG \_NCPCmd\_DS\_COMMAND::wowlan\_config

> Wakeup configuration.

#### NCP\_CMD\_POWERMGMT\_SUSPEND \_NCPCmd\_DS\_COMMAND::suspend\_config

> Enter suspend configuration.

#### WLAN\_RESET\_data \_NCPCmd\_DS\_COMMAND::reset\_config

> Wi-Fi reset config.

#### NCP\_CMD\_TWT\_REPORT \_NCPCmd\_DS\_COMMAND::twt\_report

> TWT report.

#### NCP\_CMD\_REGION\_CODE\_CFG \_NCPCmd\_DS\_COMMAND::region\_cfg

> Region code configuration structure.

#### NCP\_CMD\_ED\_MAC \_NCPCmd\_DS\_COMMAND::ed\_mac\_mode

> Regulatory commands.

#### NCP\_CMD\_RF\_TX\_ANTENNA \_NCPCmd\_DS\_COMMAND::rf\_tx\_antenna

> NCP RF TX antenna.

#### NCP\_CMD\_RF\_RX\_ANTENNA \_NCPCmd\_DS\_COMMAND::rf\_rx\_antenna

> NCP RF RX antenna.

#### NCP\_CMD\_RF\_BAND \_NCPCmd\_DS\_COMMAND::rf\_band

> NCP RF band.

#### NCP\_CMD\_RF\_BANDWIDTH \_NCPCmd\_DS\_COMMAND::rf\_bandwidth

> NCP RF bandwidth.

#### NCP\_CMD\_RF\_CHANNEL \_NCPCmd\_DS\_COMMAND::rf\_channel

> NCP RF channel.

#### NCP\_CMD\_RF\_RADIO\_MODE \_NCPCmd\_DS\_COMMAND::rf\_radio\_mode

> NCP RF radio mode.

#### NCP\_CMD\_RF\_TX\_POWER \_NCPCmd\_DS\_COMMAND::rf\_tx\_power

> NCP RF TX power.

#### NCP\_CMD\_RF\_TX\_CONT\_MODE \_NCPCmd\_DS\_COMMAND::rf\_tx\_cont\_mode

> NCP RF TX cont mode.

#### NCP\_CMD\_RF\_TX\_FRAME \_NCPCmd\_DS\_COMMAND::rf\_tx\_frame

> NCP RF TX frame.

#### NCP\_CMD\_RF\_PER \_NCPCmd\_DS\_COMMAND::rf\_per

> NCP RF per.

#### NCP\_CMD\_EU\_CRYPRTO \_NCPCmd\_DS\_COMMAND::eu\_crypto

> NCP EU (encrypt unit) cryprto.

#### NCP\_CMD\_REGISTER\_ACCESS \_NCPCmd\_DS\_COMMAND::register\_access

> Debug commands.

#### NCP\_CMD\_MEM\_STAT \_NCPCmd\_DS\_COMMAND::mem\_stat

> Memory commands.

#### NCP\_CMD\_DATE\_TIME\_CFG \_NCPCmd\_DS\_COMMAND::date\_time

> Data time.

#### NCP\_CMD\_TEMPERATURE \_NCPCmd\_DS\_COMMAND::temperature

> Chip temperature.

#### NCP\_CMD\_WLAN\_CONN \_NCPCmd\_DS\_COMMAND::wlan\_connect

> Wi-Fi connect.

#### NCP\_CMD\_MDNS\_QUERY \_NCPCmd\_DS\_COMMAND::mdns\_query

> MDNS query.

#### NCP\_EVT\_MDNS\_RESULT \_NCPCmd\_DS\_COMMAND::mdns\_result

> MDNS result.

#### NCP\_EVT\_MDNS\_RESOLVE \_NCPCmd\_DS\_COMMAND::mdns\_resolve

> MDNS resolve.

#### NCP\_EVT\_CSI\_DATA \_NCPCmd\_DS\_COMMAND::csi\_data

> CSI data.

#### NCP\_CMD\_NETWORK\_LIST \_NCPCmd\_DS\_COMMAND::network\_list

> Added network list.

#### NCP\_CMD\_NETWORK\_REMOVE \_NCPCmd\_DS\_COMMAND::network\_remove

> Network to remove.

#### NCP\_CMD\_GET\_CURRENT\_NETWORK \_NCPCmd\_DS\_COMMAND::current\_network

> Get current network

#### union { ... } \_NCPCmd\_DS\_COMMAND::params

> Command body.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_NCPCmd\_DS\_COMMAND\_BLE Struct Reference

### Data Fields

  - NCP\_COMMAND header

  - union {

  -   NCP\_CMD\_SET\_ADV\_DATA set\_adv\_data

  -   NCP\_CMD\_SET\_SCAN\_PARAM set\_scan\_parameter

  -   NCP\_CMD\_SCAN\_START scan\_start

  -   NCP\_CMD\_CONNECT connect

  -   NCP\_CMD\_SET\_DATA\_LEN set\_data\_len

  -   NCP\_CMD\_SET\_PHY set\_phy

  -   NCP\_CMD\_CONN\_PARA\_UPDATE conn\_param\_update

  -   NCP\_CMD\_ENCRYPTION conn\_encryption

  -   NCP\_CMD\_SET\_POWER\_MODE set\_pw\_mode

  -   NCP\_CMD\_SET\_ADDR set\_dev\_addr

  -   NCP\_GATT\_READ\_CMD gatt\_read\_char

  -   NCP\_CMD\_SET\_NAME set\_dev\_name

  -   NCP\_CMD\_SERVICE\_ADD host\_svc\_add

  -   NCP\_CMD\_START\_SERVICE host\_start\_svc

  -   NCP\_REGISTER\_SERVICE register\_service

  -   NCP\_SET\_VALUE\_CMD gatt\_set\_value

  -   NCP\_DISC\_PRIM\_UUID\_CMD discover\_prim

  -   NCP\_DISC\_CHRC\_UUID\_CMD discover\_chrc

  -   NCP\_CFG\_SUBSCRIBE\_CMD cfg\_subscribe

  -   NCP\_L2CAP\_CONNECT\_CMD l2cap\_connect

  -   NCP\_L2CAP\_DISCONNECT\_CMD l2cap\_disconnect

  -   NCP\_L2CAP\_SEND\_CMD l2cap\_send

  -   NCP\_L2CAP\_REGISTER\_CMD l2cap\_register

  -   NCP\_L2CAP\_METRICS\_CMD l2cap\_metrics

  -   NCP\_L2CAP\_RECEIVE\_CMD l2cap\_receive

  -   NCP\_DEVICE\_ADV\_REPORT\_EV adv\_reported

  -   NCP\_DEVICE\_CONNECTED\_EV device\_connected

  -   NCP\_DEVICE\_DISCONNECTED\_EV device\_disconnected

  -   NCP\_PASSKEY\_DISPLAY\_EV passkey\_display

  -   NCP\_IDENTITY\_RESOLVED\_EV identity\_resolved

  -   NCP\_CMD\_CONN\_PARA\_UPDATE\_EV conn\_param\_update\_ev

  -   NCP\_CMD\_PHY\_UPDATE\_EV phy\_updated\_ev

  -   NCP\_CMD\_DATA\_LEN\_UPDATE\_EV data\_len\_updated\_ev

  -   NCP\_SEC\_LEVEL\_CHANGED\_EV sec\_level\_changed

  -   NCP\_NOTIFICATION\_EV gatt\_notification

  -   NCP\_ATTR\_VALUE\_CHANGED\_EV attr\_value\_changed

  -   NCP\_CCC\_CFG\_CHANGED\_EV gatt\_ccc\_cfg\_changed\_ev

  -   NCP\_SUBSCRIPTION\_EV gatt\_subscription\_ev

  -   NCP\_DISC\_PRIM\_EV gatt\_disc\_prim\_ev

  -   NCP\_DISC\_CHRC\_EV gatt\_disc\_chrc\_ev

  -   NCP\_DISC\_ALL\_DESC\_EV gatt\_disc\_desc\_ev

  -   NCP\_L2CAP\_CONNECT\_EV l2cap\_connect\_ev

  -   NCP\_L2CAP\_DISCONNECT\_EV l2cap\_disconnect\_ev

  -   NCP\_L2CAP\_RECEIVE\_EV l2cap\_receive\_ev

  -   NCP\_START\_ADV\_RP start\_adv\_rp

  -   NCP\_ADD\_SERVICE\_RP add\_service\_rp

  -   NCP\_DISC\_PRIM\_RP discover\_prim\_rp

  -   NCP\_DISC\_CHRC\_RP discover\_chrc\_rp

  -   NCP\_DISC\_ALL\_DESC\_RP discover\_desc\_rp

  -   NCP\_NCS\_INFO\_RP ncs\_info\_rp

  - } params

### Detailed Description

This structure contains the configuration value which is used as an
input to the Bluetooth LE TLV command.

### Field Documentation

#### NCP\_COMMAND \_NCPCmd\_DS\_COMMAND\_BLE::header

> Command Header : Command

#### NCP\_CMD\_SET\_ADV\_DATA \_NCPCmd\_DS\_COMMAND\_BLE::set\_adv\_data

> Set NCP Bluetooth LE advertising data

#### NCP\_CMD\_SET\_SCAN\_PARAM \_NCPCmd\_DS\_COMMAND\_BLE::set\_scan\_parameter

> Set NCP Bluetooth LE scan parameter

#### NCP\_CMD\_SCAN\_START \_NCPCmd\_DS\_COMMAND\_BLE::scan\_start

> NCP Bluetooth LE start scanning

#### NCP\_CMD\_CONNECT \_NCPCmd\_DS\_COMMAND\_BLE::connect

> NCP Bluetooth LE connect/disconnect

#### NCP\_CMD\_SET\_DATA\_LEN \_NCPCmd\_DS\_COMMAND\_BLE::set\_data\_len

> Set NCP Bluetooth LE data len

#### NCP\_CMD\_SET\_PHY \_NCPCmd\_DS\_COMMAND\_BLE::set\_phy

> Set NCP Bluetooth LE set Bluetooth PHY

#### NCP\_CMD\_CONN\_PARA\_UPDATE \_NCPCmd\_DS\_COMMAND\_BLE::conn\_param\_update

> NCP Bluetooth LE update connection parameters

#### NCP\_CMD\_ENCRYPTION \_NCPCmd\_DS\_COMMAND\_BLE::conn\_encryption

> NCP Bluetooth LE connect encryption

#### NCP\_CMD\_SET\_POWER\_MODE \_NCPCmd\_DS\_COMMAND\_BLE::set\_pw\_mode

> NCP Bluetooth LE set power mode

#### NCP\_CMD\_SET\_ADDR \_NCPCmd\_DS\_COMMAND\_BLE::set\_dev\_addr

> NCP Bluetooth LE set device address

#### NCP\_GATT\_READ\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_read\_char

> NCP Bluetooth LE read characteristic

#### NCP\_CMD\_SET\_NAME \_NCPCmd\_DS\_COMMAND\_BLE::set\_dev\_name

> NCP Bluetooth LE set device name

#### NCP\_CMD\_SERVICE\_ADD \_NCPCmd\_DS\_COMMAND\_BLE::host\_svc\_add

> NCP Bluetooth LE gatt add host service

#### NCP\_CMD\_START\_SERVICE \_NCPCmd\_DS\_COMMAND\_BLE::host\_start\_svc

> NCP Bluetooth LE start service at host side

#### NCP\_REGISTER\_SERVICE \_NCPCmd\_DS\_COMMAND\_BLE::register\_service

> NCP Bluetooth LE gatt register service

#### NCP\_SET\_VALUE\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_set\_value

> NCP Bluetooth LE gatt set characteristic/descriptor service

#### NCP\_DISC\_PRIM\_UUID\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::discover\_prim

> NCP Bluetooth LE gatt discover primary Service

#### NCP\_DISC\_CHRC\_UUID\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::discover\_chrc

> NCP Bluetooth LE gatt discover characteristics

#### NCP\_CFG\_SUBSCRIBE\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::cfg\_subscribe

> NCP Bluetooth LE gatt configure service to indicate characteristic
> value to client

#### NCP\_L2CAP\_CONNECT\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_connect

> NCP Bluetooth LE l2cap connect

#### NCP\_L2CAP\_DISCONNECT\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_disconnect

> NCP Bluetooth LE l2cap disconnect

#### NCP\_L2CAP\_SEND\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_send

> NCP Bluetooth LE l2cap send

#### NCP\_L2CAP\_REGISTER\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_register

> NCP Bluetooth LE l2cap register

#### NCP\_L2CAP\_METRICS\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_metrics

> NCP Bluetooth LE l2cap metrics

#### NCP\_L2CAP\_RECEIVE\_CMD \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_receive

> NCP Bluetooth LE l2cap receive

#### NCP\_DEVICE\_ADV\_REPORT\_EV \_NCPCmd\_DS\_COMMAND\_BLE::adv\_reported

> NCP Bluetooth LE advertising reported event

#### NCP\_DEVICE\_CONNECTED\_EV \_NCPCmd\_DS\_COMMAND\_BLE::device\_connected

> NCP Bluetooth LE connected event

#### NCP\_DEVICE\_DISCONNECTED\_EV \_NCPCmd\_DS\_COMMAND\_BLE::device\_disconnected

> NCP Bluetooth LE disconnected event

#### NCP\_PASSKEY\_DISPLAY\_EV \_NCPCmd\_DS\_COMMAND\_BLE::passkey\_display

> NCP Bluetooth LE passkey display event

#### NCP\_IDENTITY\_RESOLVED\_EV \_NCPCmd\_DS\_COMMAND\_BLE::identity\_resolved

> NCP Bluetooth LE remote identity address resolved event

#### NCP\_CMD\_CONN\_PARA\_UPDATE\_EV \_NCPCmd\_DS\_COMMAND\_BLE::conn\_param\_update\_ev

> NCP Bluetooth LE connection update event

#### NCP\_CMD\_PHY\_UPDATE\_EV \_NCPCmd\_DS\_COMMAND\_BLE::phy\_updated\_ev

> NCP Bluetooth LE phy update event

#### NCP\_CMD\_DATA\_LEN\_UPDATE\_EV \_NCPCmd\_DS\_COMMAND\_BLE::data\_len\_updated\_ev

> NCP Bluetooth LE data length update event

#### NCP\_SEC\_LEVEL\_CHANGED\_EV \_NCPCmd\_DS\_COMMAND\_BLE::sec\_level\_changed

> NCP Bluetooth LE security level changed event

#### NCP\_NOTIFICATION\_EV \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_notification

> NCP Bluetooth LE gatt notification receive event

#### NCP\_ATTR\_VALUE\_CHANGED\_EV \_NCPCmd\_DS\_COMMAND\_BLE::attr\_value\_changed

> NCP Bluetooth LE gatt attribute value changed event

#### NCP\_CCC\_CFG\_CHANGED\_EV \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_ccc\_cfg\_changed\_ev

> NCP Bluetooth LE gatt client characteristic configuration changed
> event

#### NCP\_SUBSCRIPTION\_EV \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_subscription\_ev

> NCP Bluetooth LE gatt client subscription event

#### NCP\_DISC\_PRIM\_EV \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_disc\_prim\_ev

> NCP Bluetooth LE gatt discover primary service event

#### NCP\_DISC\_CHRC\_EV \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_disc\_chrc\_ev

> NCP Bluetooth LE gatt discover characteristic event

#### NCP\_DISC\_ALL\_DESC\_EV \_NCPCmd\_DS\_COMMAND\_BLE::gatt\_disc\_desc\_ev

> NCP Bluetooth LE gatt discover description event

#### NCP\_L2CAP\_CONNECT\_EV \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_connect\_ev

> NCP Bluetooth LE l2cap connect event

#### NCP\_L2CAP\_DISCONNECT\_EV \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_disconnect\_ev

> NCP Bluetooth LE l2cap disconnect event

#### NCP\_L2CAP\_RECEIVE\_EV \_NCPCmd\_DS\_COMMAND\_BLE::l2cap\_receive\_ev

> NCP Bluetooth LE l2cap receive event

#### NCP\_START\_ADV\_RP \_NCPCmd\_DS\_COMMAND\_BLE::start\_adv\_rp

> NCP Bluetooth LE start advertising response

#### NCP\_ADD\_SERVICE\_RP \_NCPCmd\_DS\_COMMAND\_BLE::add\_service\_rp

> NCP Bluetooth LE gatt add service attribute response

#### NCP\_DISC\_PRIM\_RP \_NCPCmd\_DS\_COMMAND\_BLE::discover\_prim\_rp

> NCP Bluetooth LE gatt discover primary service response

#### NCP\_DISC\_CHRC\_RP \_NCPCmd\_DS\_COMMAND\_BLE::discover\_chrc\_rp

> NCP Bluetooth LE gatt discover characteristics response

#### NCP\_DISC\_ALL\_DESC\_RP \_NCPCmd\_DS\_COMMAND\_BLE::discover\_desc\_rp

> NCP Bluetooth LE gatt discover descriptors response

#### NCP\_NCS\_INFO\_RP \_NCPCmd\_DS\_COMMAND\_BLE::ncs\_info\_rp

> NCP Bluetooth LE commissioning ssid/password receive response

#### union { ... } \_NCPCmd\_DS\_COMMAND\_BLE::params

> Command Body

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## \_ping\_msg\_t Struct Reference

### Data Fields

  - uint16\_t size

  - uint32\_t count

  - uint32\_t timeout

  - uint32\_t handle

  - char ip\_addr \[IP\_ADDR\_LEN\]

  - uint32\_t port

### Detailed Description

This structure is used for the ping command configuration.

### Field Documentation

#### uint16\_t \_ping\_msg\_t::size

> Packet size.

#### uint32\_t \_ping\_msg\_t::count

> Packet count.

#### uint32\_t \_ping\_msg\_t::timeout

> Timeout in seconds.

#### uint32\_t \_ping\_msg\_t::handle

> Socket handle index number.

#### char \_ping\_msg\_t::ip\_addr\[IP\_ADDR\_LEN\]

> Peer IP address.

#### uint32\_t \_ping\_msg\_t::port

> Peer port number.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## \_ping\_res Struct Reference

### Data Fields

  - int seq\_no

  - int echo\_resp

  - uint32\_t time

  - uint32\_t recvd

  - int ttl

  - char ip\_addr \[IP\_ADDR\_LEN\]

  - uint16\_t size

### Detailed Description

This structure is used for ping command response message.

### Field Documentation

#### int \_ping\_res::seq\_no

> Sequence number.

#### int \_ping\_res::echo\_resp

> Echo response.

#### uint32\_t \_ping\_res::time

> Time interval.

#### uint32\_t \_ping\_res::recvd

> Count of received packet.

#### int \_ping\_res::ttl

> Time to live

#### char \_ping\_res::ip\_addr\[IP\_ADDR\_LEN\]

> IP address of the peer device.

#### uint16\_t \_ping\_res::size

> Packet size.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## \_PMF\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t mfpc

  - uint8\_t mfpr

### Detailed Description

This structure is used for PMF (protected management frame)
configuration.

### Field Documentation

#### TypeHeader\_t \_PMF\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_PMF\_ParamSet\_t::mfpc

> MFPC (management frame protection capable) flag,
> 
> 1: enable,
> 
> 0: disable.

#### uint8\_t \_PMF\_ParamSet\_t::mfpr

> MFPR (management frame protection required) flag,
> 
> 1: enable,
> 
> 0: disable.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_PTR\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - char instance\_name \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

  - char service\_type \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

  - char proto \[8\]

### Detailed Description

This structure is used for pointer records configuration.

### Field Documentation

#### TypeHeader\_t \_PTR\_ParamSet\_t::header

> Header type and size information.

#### char \_PTR\_ParamSet\_t::instance\_name\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> Instance name, used to distinguish between different service instances
> of the same type.

#### char \_PTR\_ParamSet\_t::service\_type\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> The type of service and the transport protocol are identified, for
> example, \_http.\_tcp indicates an HTTP service that uses the TCP
> protocol.

#### char \_PTR\_ParamSet\_t::proto\[8\]

> The protocol type of service, for example, the TCP/UDP protocol.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_Pwe\_Derivation\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t pwe\_derivation

### Detailed Description

This structure is used for PWE (password element) derivation
configuration.

### Field Documentation

#### TypeHeader\_t \_Pwe\_Derivation\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_Pwe\_Derivation\_ParamSet\_t::pwe\_derivation

> sae\_pwe - SAE mechanism for PWE derivation.
> 
> 0: hunting-and-pecking loop only,
> 
> 1: hash-to-element only,
> 
> 2: both hunting-and-pecking loop and hash-to-element enabled.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_QUERY\_A\_CFG Struct Reference

### Data Fields

  - char name \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

### Detailed Description

This structure is used for domain name configuration.

### Field Documentation

#### char \_QUERY\_A\_CFG::name\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> Domain name, like "wifi-http.local".

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_QUERY\_PTR\_CFG Struct Reference

### Data Fields

  - char service \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

  - uint16\_t proto

### Detailed Description

This structure is used for service protocol configuration.

### Field Documentation

#### char \_QUERY\_PTR\_CFG::service\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> Type of service, like "http".

#### uint16\_t \_QUERY\_PTR\_CFG::proto

> Protocol type,
> 
> 0: UDP,
> 
> 1: TCP.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_Security\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t type

  - uint8\_t password\_len

  - char password \[1\]

### Detailed Description

This structure is used for security password configuration.

### Field Documentation

#### TypeHeader\_t \_Security\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_Security\_ParamSet\_t::type

> Wi-Fi security type.

#### uint8\_t \_Security\_ParamSet\_t::password\_len

> Wi-Fi security password length.

#### char \_Security\_ParamSet\_t::password\[1\]

> Wi-Fi security password string.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_SRV\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - char host\_name \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

  - uint16\_t port

  - char target \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

### Detailed Description

This structure is used for service records configuration.

### Field Documentation

#### TypeHeader\_t \_SRV\_ParamSet\_t::header

> Header type and size information.

#### char \_SRV\_ParamSet\_t::host\_name\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> Host name.

#### uint16\_t \_SRV\_ParamSet\_t::port

> Service port.

#### char \_SRV\_ParamSet\_t::target\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> Target name.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_SSID\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - char ssid \[IEEEtypes\_SSID\_SIZE+1\]

### Detailed Description

This structure is used for SSID configuration.

### Field Documentation

#### TypeHeader\_t \_SSID\_ParamSet\_t::header

> Header type and size information.

#### char \_SSID\_ParamSet\_t::ssid\[IEEEtypes\_SSID\_SIZE+1\]

> SSID (service set identifier)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_Transition\_Disable\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t transition\_disable

### Detailed Description

This structure is used for transition disable flag configuration.

### Field Documentation

#### TypeHeader\_t \_Transition\_Disable\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_Transition\_Disable\_ParamSet\_t::transition\_disable

> Transition disable flag.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_TXT\_ParamSet\_t Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t txt\_len

  - char txt \[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

### Detailed Description

This structure is used for txt resource configuration.

### Field Documentation

#### TypeHeader\_t \_TXT\_ParamSet\_t::header

> Header type and size information.

#### uint8\_t \_TXT\_ParamSet\_t::txt\_len

> txt value length.

#### char \_TXT\_ParamSet\_t::txt\[NCP\_MDNS\_LABEL\_MAX\_LEN+1\]

> txt string.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## \_wifi\_csi\_config\_params\_t Struct Reference

### Data Fields

  - uint8\_t bss\_type

  - uint16\_t csi\_enable

  - uint32\_t head\_id

  - uint32\_t tail\_id

  - uint8\_t csi\_filter\_cnt

  - uint8\_t chip\_id

  - uint8\_t band\_config

  - uint8\_t channel

  - uint8\_t csi\_monitor\_enable

  - uint8\_t ra4us

  - wifi\_csi\_filter\_t csi\_filter \[CSI\_FILTER\_MAX\]

### Detailed Description

This structure is used for Wi-Fi CSI configuration.

### Field Documentation

#### uint8\_t \_wifi\_csi\_config\_params\_t::bss\_type

> BSS type,
> 
> 0: STA;
> 
> 1: UAP.

#### uint16\_t \_wifi\_csi\_config\_params\_t::csi\_enable

> CSI enable flag,
> 
> 1: enable,
> 
> 2: disable.

#### uint32\_t \_wifi\_csi\_config\_params\_t::head\_id

> Header ID

#### uint32\_t \_wifi\_csi\_config\_params\_t::tail\_id

> Tail ID

#### uint8\_t \_wifi\_csi\_config\_params\_t::csi\_filter\_cnt

> Number of CSI filters

#### uint8\_t \_wifi\_csi\_config\_params\_t::chip\_id

> Chip ID

#### uint8\_t \_wifi\_csi\_config\_params\_t::band\_config

> band config

#### uint8\_t \_wifi\_csi\_config\_params\_t::channel

> Channel num

#### uint8\_t \_wifi\_csi\_config\_params\_t::csi\_monitor\_enable

> Enable getting CSI data on special channel

#### uint8\_t \_wifi\_csi\_config\_params\_t::ra4us

> CSI data received in cfg channel with mac addr filter, not only RA is
> us or other.

#### wifi\_csi\_filter\_t \_wifi\_csi\_config\_params\_t::csi\_filter\[CSI\_FILTER\_MAX\]

> CSI filters.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## \_wifi\_csi\_filter\_t Struct Reference

### Data Fields

  - uint8\_t mac\_addr \[MLAN\_MAC\_ADDR\_LENGTH\]

  - uint8\_t pkt\_type

  - uint8\_t subtype

  - uint8\_t flags

### Detailed Description

This structure is used for Wi-Fi CSI (channel state information) filter
configuration.

### Field Documentation

#### uint8\_t \_wifi\_csi\_filter\_t::mac\_addr\[MLAN\_MAC\_ADDR\_LENGTH\]

> Source address of the packet to receive.

#### uint8\_t \_wifi\_csi\_filter\_t::pkt\_type

> Packet type of the interested CSI.

#### uint8\_t \_wifi\_csi\_filter\_t::subtype

> Packet subtype of the interested CSI.

#### uint8\_t \_wifi\_csi\_filter\_t::flags

> Other filter flags

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## \_wlan\_date\_time\_t Struct Reference

### Data Fields

  - uint32\_t action

  - uint16\_t year

  - uint8\_t month

  - uint8\_t day

  - uint8\_t hour

  - uint8\_t minute

  - uint8\_t second

### Detailed Description

This structure is used for date and time configuration.

### Field Documentation

#### uint32\_t \_wlan\_date\_time\_t::action

> Action type,
> 
> 0: get,
> 
> 1: set.

#### uint16\_t \_wlan\_date\_time\_t::year

> Year: range from 1970 to 2099.

#### uint8\_t \_wlan\_date\_time\_t::month

> Month: range from 1 to 12.

#### uint8\_t \_wlan\_date\_time\_t::day

> Day: range from 1 to 31 (depending on month).

#### uint8\_t \_wlan\_date\_time\_t::hour

> Hour: range from 0 to 23.

#### uint8\_t \_wlan\_date\_time\_t::minute

> Minute: range from 0 to 59.

#### uint8\_t \_wlan\_date\_time\_t::second

> Minute: range from 0 to 59.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

#### 

## adv\_data\_t Struct Reference

### Data Fields

  - uint8\_t data\_len

  - uint8\_t type

  - const uint8\_t \* data

### Detailed Description

NCP Bluetooth LE adv data type structure

### Field Documentation

#### uint8\_t adv\_data\_t::data\_len

> adv data length

#### uint8\_t adv\_data\_t::type

> adv data type

#### const uint8\_t\* adv\_data\_t::data

> adv data

#### The documentation for this struct was generated from the following file:

  - service.h

#### 

## gap\_bond\_lost\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the value of the Bluetooth LE lost bond event
which indicates the signature check has failed and the client need pair
again to reestablish the bond.

### Field Documentation

#### uint8\_t gap\_bond\_lost\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_bond\_lost\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_bond\_pairing\_failed\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t reason

### Detailed Description

This structure contains the value of Bluetooth LE pairing failed event
which the secure pairing process has failed.

### Field Documentation

#### uint8\_t gap\_bond\_pairing\_failed\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_bond\_pairing\_failed\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gap\_bond\_pairing\_failed\_ev::reason

> pairing failed reason
> 
> 0: Security procedure successful
> 
> 1: Authentication failed
> 
> 2: PIN or encryption key is missing
> 
> 3: OOB data is not available
> 
> 4: The requested security level could not be reached
> 
> 5: Pairing is not supported
> 
> 6: Pairing is not allowed
> 
> 7: Invalid parameters
> 
> 8: Distributed Key Rejected
> 
> 9: Pairing failed but the exact reason could not be specified

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_conn\_param\_update\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t interval\_min

  - uint16\_t interval\_max

  - uint16\_t latency

  - uint16\_t timeout

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE update connection command.

### Field Documentation

#### uint8\_t gap\_conn\_param\_update\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_conn\_param\_update\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gap\_conn\_param\_update\_cmd::interval\_min

> connection minimal interval
> 
> Minimum value for the connection interval. Must be less than or equal
> to maximum connection interval

#### uint16\_t gap\_conn\_param\_update\_cmd::interval\_max

> connection maximum interval
> 
> Maximum value for the connection interval. Must be greater than or
> equal to minimum connection interval

#### uint16\_t gap\_conn\_param\_update\_cmd::latency

> connection latency
> 
> Maximum peripheral latency for the connection in number of subrated
> connection events

#### uint16\_t gap\_conn\_param\_update\_cmd::timeout

> connection timeout
> 
> Supervision timeout for the Bluetooth LE link

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_conn\_param\_update\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t interval

  - uint16\_t latency

  - uint16\_t timeout

### Detailed Description

This structure contains the value of the Bluetooth LE connection update
complete event which indicate the connection update procedure has
completed.

### Field Documentation

#### uint8\_t gap\_conn\_param\_update\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_conn\_param\_update\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gap\_conn\_param\_update\_ev::interval

> connection interval

#### uint16\_t gap\_conn\_param\_update\_ev::latency

> connection latency
> 
> Maximum peripheral latency for the connection in number of subrated
> connection events

#### uint16\_t gap\_conn\_param\_update\_ev::timeout

> connection timeout
> 
> Supervision timeout for the Bluetooth LE link

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_connect\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE connect command.

### Field Documentation

#### uint8\_t gap\_connect\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_connect\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_data\_len\_updated\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t tx\_max\_len

  - uint16\_t tx\_max\_time

  - uint16\_t rx\_max\_len

  - uint16\_t rx\_max\_time

### Detailed Description

This structure contains the value of the Bluetooth LE data length change
event which notifies the Host of a change to either the maximum LL Data
PDU Payload length or the maximum transmission time of packets
containing LL Data PDUs in either direction.

### Field Documentation

#### uint8\_t gap\_data\_len\_updated\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_data\_len\_updated\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gap\_data\_len\_updated\_ev::tx\_max\_len

> Maximum data length of TX connection

#### uint16\_t gap\_data\_len\_updated\_ev::tx\_max\_time

> Maximum time of TX connection

#### uint16\_t gap\_data\_len\_updated\_ev::rx\_max\_len

> Maximum length of RX connection

#### uint16\_t gap\_data\_len\_updated\_ev::rx\_max\_time

> Maximum time of RX connection

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_device\_connected\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t interval

  - uint16\_t latency

  - uint16\_t timeout

### Detailed Description

This structure contains the value of the Bluetooth LE connection
complete event which indicates a new connection has been created.

### Field Documentation

#### uint8\_t gap\_device\_connected\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_device\_connected\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gap\_device\_connected\_ev::interval

> connection interval

#### uint16\_t gap\_device\_connected\_ev::latency

> connection latency
> 
> Maximum peripheral latency for the connection in number of subrated
> connection events

#### uint16\_t gap\_device\_connected\_ev::timeout

> connection timeout
> 
> Supervision timeout for the Bluetooth LE link

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_device\_disconnected\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the value of the Bluetooth LE disconnection
complete event which indicates a connection is terminated.

### Field Documentation

#### uint8\_t gap\_device\_disconnected\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_device\_disconnected\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_device\_found\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - int8\_t rssi

  - uint8\_t flags

  - uint16\_t eir\_data\_len

  - uint8\_t eir\_data \[\]

### Detailed Description

This structure contains the value of the Bluetooth LE advertising report
event which indicates one or more Bluetooth devices have responded to an
active scan or have broadcast advertisements that were received during a
passive scan.

### Field Documentation

#### uint8\_t gap\_device\_found\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_device\_found\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### int8\_t gap\_device\_found\_ev::rssi

> scan rssi

#### uint8\_t gap\_device\_found\_ev::flags

> Bluetooth LE device found flag
> 
> 0x01: GAP\_DEVICE\_FOUND\_FLAG\_RSSI
> 
> 0x02: GAP\_DEVICE\_FOUND\_FLAG\_AD
> 
> 0x04: GAP\_DEVICE\_FOUND\_FLAG\_SD

#### uint16\_t gap\_device\_found\_ev::eir\_data\_len

> scan data length

#### uint8\_t gap\_device\_found\_ev::eir\_data\[\]

> scan data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_disconnect\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE disconnect command.

### Field Documentation

#### uint8\_t gap\_disconnect\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_disconnect\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_identity\_resolved\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t identity\_address\_type

  - uint8\_t identity\_address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the value of the Bluetooth LE identity resolved
event which indicates that the remote host Identity Address has been
resolved.

### Field Documentation

#### uint8\_t gap\_identity\_resolved\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_identity\_resolved\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gap\_identity\_resolved\_ev::identity\_address\_type

> Bluetooth LE identity address type
> 
> 2: public identity address
> 
> 3: random identity address

#### uint8\_t gap\_identity\_resolved\_ev::identity\_address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE identity address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_pair\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE pair command.

### Field Documentation

#### uint8\_t gap\_pair\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_pair\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_passkey\_display\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint32\_t passkey

### Detailed Description

This structure contains the value of the Bluetooth LE user passkey
notification event which is used to provide a passkey for the Host to
display to the user.

### Field Documentation

#### uint8\_t gap\_passkey\_display\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_passkey\_display\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint32\_t gap\_passkey\_display\_ev::passkey

> device passkey number

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_phy\_updated\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t tx\_phy

  - uint8\_t rx\_phy

### Detailed Description

This structure contains the value of the Bluetooth LE PHY update
complete event which indicate that the controller has changed the
transmitter PHY or receiver PHY in use.

### Field Documentation

#### uint8\_t gap\_phy\_updated\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_phy\_updated\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gap\_phy\_updated\_ev::tx\_phy

> Bluetooth TX PHY
> 
> 1: 1M PHY
> 
> 2: 2M PHY
> 
> 4: Coded PHY

#### uint8\_t gap\_phy\_updated\_ev::rx\_phy

> Bluetooth RX PHY
> 
> 1: 1M PHY
> 
> 2: 2M PHY
> 
> 4: Coded PHY

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_sec\_level\_changed\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t sec\_level

### Detailed Description

This structure contains the value of the Bluetooth LE security level
changed event which is sent when the connection security level has
changed.

### Field Documentation

#### uint8\_t gap\_sec\_level\_changed\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_sec\_level\_changed\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gap\_sec\_level\_changed\_ev::sec\_level

> connection security level
> 
> 0x01: No encryption and no authentication
> 
> 0x02: Encryption and no authentication (no MITM)
> 
> 0x03: Encryption and authentication (MITM)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_set\_adv\_data\_cmd Struct Reference

### Data Fields

  - uint8\_t adv\_data\_len

  - uint8\_t data \[31\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set advertising data command.

### Field Documentation

#### uint8\_t gap\_set\_adv\_data\_cmd::adv\_data\_len

> advertising data length

#### uint8\_t gap\_set\_adv\_data\_cmd::data\[31\]

> Advertising data to set
> 
> advertising data follows the format:
> 
> struct bt\_data {
> 
> U8 len;
> 
> U8 type;
> 
> const U8 \*data;
> 
> }

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_set\_data\_len\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t time\_flag

  - uint16\_t tx\_max\_len

  - uint16\_t tx\_max\_time

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set advertising data length command.

### Field Documentation

#### uint8\_t gap\_set\_data\_len\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_set\_data\_len\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gap\_set\_data\_len\_cmd::time\_flag

> option to use tx\_max\_time or to use system calculate maximum TX
> transmit time
> 
> 0: use system maximum TX transmit time
> 
> 1: use customized maximum TX transmit time

#### uint16\_t gap\_set\_data\_len\_cmd::tx\_max\_len

> Maximum data length to transmit

#### uint16\_t gap\_set\_data\_len\_cmd::tx\_max\_time

> Maximum TX transmit time

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_set\_filter\_list Struct Reference

### Data Fields

  - uint8\_t cnt

  - le\_addr\_t addr

### Detailed Description

This structure contains the configuration values of Bluetooth LE filter
list.

### Field Documentation

#### uint8\_t gap\_set\_filter\_list::cnt

> The number of filter addresses to set

#### le\_addr\_t gap\_set\_filter\_list::addr

> list of filter addresses

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_set\_phy\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t options

  - uint8\_t pref\_tx\_phy

  - uint8\_t pref\_rx\_phy

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set PHY command.

### Field Documentation

#### uint8\_t gap\_set\_phy\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_set\_phy\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gap\_set\_phy\_cmd::options

> Connection PHY options
> 
> 0: BT\_CONN\_LE\_PHY\_OPT\_NONE (Convenience value when no options are
> specified)
> 
> bit0: BT\_CONN\_LE\_PHY\_OPT\_CODED\_S2 (LE Coded using S=2 coding
> preferred when transmitting)
> 
> bit1: BT\_CONN\_LE\_PHY\_OPT\_CODED\_S8 (LE Coded using S=8 coding
> preferred when transmitting)

#### uint8\_t gap\_set\_phy\_cmd::pref\_tx\_phy

> Bitmask of preferred transmit PHYs
> 
> 0: BT\_GAP\_LE\_PHY\_NONE (Convenience macro for when no PHY is set)
> 
> bit0: BT\_GAP\_LE\_PHY\_1M (LE 1M PHY)
> 
> bit1: BT\_GAP\_LE\_PHY\_2M (LE 2M PHY)
> 
> bit2: BT\_GAP\_LE\_PHY\_CODED (LE Coded PHY)

#### uint8\_t gap\_set\_phy\_cmd::pref\_rx\_phy

> Bitmask of preferred receive PHYs
> 
> 0: BT\_GAP\_LE\_PHY\_NONE (Convenience macro for when no PHY is set)
> 
> bit0: BT\_GAP\_LE\_PHY\_1M (LE 1M PHY)
> 
> bit1: BT\_GAP\_LE\_PHY\_2M (LE 2M PHY)
> 
> bit2: BT\_GAP\_LE\_PHY\_CODED (LE Coded PHY)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_set\_scan\_param\_cmd Struct Reference

### Data Fields

  - uint32\_t options

  - uint16\_t interval

  - uint16\_t window

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set scan parameter command.

### Field Documentation

#### uint32\_t gap\_set\_scan\_param\_cmd::options

> Bit-field of scanning options.

#### uint16\_t gap\_set\_scan\_param\_cmd::interval

> Scan interval (N \* 0.625 ms)

#### uint16\_t gap\_set\_scan\_param\_cmd::window

> Scan window (N \* 0.625 ms)

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_start\_advertising\_cmd Struct Reference

### Data Fields

  - uint8\_t adv\_data\_len

  - uint8\_t scan\_rsp\_len

  - uint8\_t adv\_sr\_data \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE advertising command.

### Field Documentation

#### uint8\_t gap\_start\_advertising\_cmd::adv\_data\_len

> advertising data length

#### uint8\_t gap\_start\_advertising\_cmd::scan\_rsp\_len

> scan response length

#### uint8\_t gap\_start\_advertising\_cmd::adv\_sr\_data\[\]

> advertising/scan response data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_start\_advertising\_rp Struct Reference

### Data Fields

  - uint32\_t current\_settings

### Detailed Description

This structure contains the command response values of the Bluetooth LE
start advertising command.

### Field Documentation

#### uint32\_t gap\_start\_advertising\_rp::current\_settings

> device current setting
> 
> bit0: GAP\_SETTINGS\_POWERED
> 
> bit1: GAP\_SETTINGS\_CONNECTABLE
> 
> bit2: GAP\_SETTINGS\_FAST\_CONNECTABLE
> 
> bit3: GAP\_SETTINGS\_DISCOVERABLE
> 
> bit4: GAP\_SETTINGS\_BONDABLE
> 
> bit5: GAP\_SETTINGS\_LINK\_SEC\_3
> 
> bit6: GAP\_SETTINGS\_SSP
> 
> bit7: GAP\_SETTINGS\_BREDR
> 
> bit8: GAP\_SETTINGS\_HS
> 
> bit9: GAP\_SETTINGS\_LE
> 
> bit10: GAP\_SETTINGS\_ADVERTISING
> 
> bit11: GAP\_SETTINGS\_SC
> 
> bit12: GAP\_SETTINGS\_DEBUG\_KEYS
> 
> bit13: GAP\_SETTINGS\_PRIVACY
> 
> bit14: GAP\_SETTINGS\_CONTROLLER\_CONFIG
> 
> bit15: GAP\_SETTINGS\_STATIC\_ADDRESS

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_start\_discovery\_cmd Struct Reference

### Data Fields

  - uint8\_t flags

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE start discovery command.

### Field Documentation

#### uint8\_t gap\_start\_discovery\_cmd::flags

> discovery setting
> 
> 0x01: discovery le
> 
> 0x02: discovery bredr
> 
> 0x04: limit discovery
> 
> 0x08: active scan
> 
> 0x10: observe
> 
> 0x20: discovery own address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_stop\_advertising\_rp Struct Reference

### Data Fields

  - uint32\_t current\_settings

### Detailed Description

This structure contains the command response values of the Bluetooth LE
stop advertising command.

### Field Documentation

#### uint32\_t gap\_stop\_advertising\_rp::current\_settings

> device current setting
> 
> bit0: GAP\_SETTINGS\_POWERED
> 
> bit1: GAP\_SETTINGS\_CONNECTABLE
> 
> bit2: GAP\_SETTINGS\_FAST\_CONNECTABLE
> 
> bit3: GAP\_SETTINGS\_DISCOVERABLE
> 
> bit4: GAP\_SETTINGS\_BONDABLE
> 
> bit5: GAP\_SETTINGS\_LINK\_SEC\_3
> 
> bit6: GAP\_SETTINGS\_SSP
> 
> bit7: GAP\_SETTINGS\_BREDR
> 
> bit8: GAP\_SETTINGS\_HS
> 
> bit9: GAP\_SETTINGS\_LE
> 
> bit10: GAP\_SETTINGS\_ADVERTISING
> 
> bit11: GAP\_SETTINGS\_SC
> 
> bit12: GAP\_SETTINGS\_DEBUG\_KEYS
> 
> bit13: GAP\_SETTINGS\_PRIVACY
> 
> bit14: GAP\_SETTINGS\_CONTROLLER\_CONFIG
> 
> bit15: GAP\_SETTINGS\_STATIC\_ADDRESS

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gap\_unpair\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE unpair command.

### Field Documentation

#### uint8\_t gap\_unpair\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gap\_unpair\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_characteristic\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint16\_t svc\_id

  - uint8\_t properties

  - uint16\_t permissions

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE add characteristic command.

### Field Documentation

#### TypeHeader\_t gatt\_add\_characteristic\_cmd::header

> Header of the type tlv

#### uint16\_t gatt\_add\_characteristic\_cmd::svc\_id

> service id

#### uint8\_t gatt\_add\_characteristic\_cmd::properties

> characteristic properties

#### uint16\_t gatt\_add\_characteristic\_cmd::permissions

> characteristic permission

#### uint8\_t gatt\_add\_characteristic\_cmd::uuid\_length

> characteristic length

#### uint8\_t gatt\_add\_characteristic\_cmd::uuid\[SERVER\_MAX\_UUID\_LEN\]

> characteristic UUID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_characteristic\_rp Struct Reference

### Data Fields

  - uint16\_t char\_id

### Detailed Description

This structure is returned as an output from the Bluetooth LE add
characteristic command.

### Field Documentation

#### uint16\_t gatt\_add\_characteristic\_rp::char\_id

> characteristic ID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_descriptor\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint16\_t char\_id

  - uint16\_t permissions

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE add descriptor command.

### Field Documentation

#### TypeHeader\_t gatt\_add\_descriptor\_cmd::header

> Header of the type tlv

#### uint16\_t gatt\_add\_descriptor\_cmd::char\_id

> characteristic id

#### uint16\_t gatt\_add\_descriptor\_cmd::permissions

> descriptor permission

#### uint8\_t gatt\_add\_descriptor\_cmd::uuid\_length

> descriptor length

#### uint8\_t gatt\_add\_descriptor\_cmd::uuid\[SERVER\_MAX\_UUID\_LEN\]

> descriptor UUID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_descriptor\_rp Struct Reference

### Data Fields

  - uint16\_t desc\_id

### Detailed Description

This structure is returned as an output from the Bluetooth LE add
descriptor command.

### Field Documentation

#### uint16\_t gatt\_add\_descriptor\_rp::desc\_id

> descriptor id

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_included\_service\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint16\_t svc\_id

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE add include service command.

### Field Documentation

#### TypeHeader\_t gatt\_add\_included\_service\_cmd::header

> Header of the type tlv

#### uint16\_t gatt\_add\_included\_service\_cmd::svc\_id

> service UUID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_included\_service\_rp Struct Reference

### Data Fields

  - uint16\_t included\_service\_id

### Detailed Description

This structure is returned as an output from the Bluetooth LE add
include service command.

### Field Documentation

#### uint16\_t gatt\_add\_included\_service\_rp::included\_service\_id

> include service ID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_service\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t type

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE add service command.

### Field Documentation

#### TypeHeader\_t gatt\_add\_service\_cmd::header

> Header of the type tlv

#### uint8\_t gatt\_add\_service\_cmd::type

> Bluetooth LE service type
> 
> 0: primary service
> 
> 1: secondary service

#### uint8\_t gatt\_add\_service\_cmd::uuid\_length

> service UUID length
> 
> 0x02: UUID type 16-bit
> 
> 0x10: UUID type 128-bit

#### uint8\_t gatt\_add\_service\_cmd::uuid\[SERVER\_MAX\_UUID\_LEN\]

> service UUID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_add\_service\_rp Struct Reference

### Data Fields

  - uint16\_t svc\_id

### Detailed Description

This structure is returned as an output from the Bluetooth LE add
service command.

### Field Documentation

#### uint16\_t gatt\_add\_service\_rp::svc\_id

> attribute handle of added new service

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_attr Struct Reference

### Data Fields

  - uint16\_t handle

  - uint8\_t permission

  - uint8\_t type\_length

  - uint8\_t type \[\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
attributes.

### Field Documentation

#### uint16\_t gatt\_attr::handle

> attribute handle

#### uint8\_t gatt\_attr::permission

> attribute permission

#### uint8\_t gatt\_attr::type\_length

> attribute type length

#### uint8\_t gatt\_attr::type\[\]

> attribute type list

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_attr\_value\_changed\_ev Struct Reference

### Data Fields

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the value of the Bluetooth LE GATT attribute
value changed event which notifies the changed attributes to the client.

### Field Documentation

#### uint16\_t gatt\_attr\_value\_changed\_ev::handle

> attribute handle

#### uint16\_t gatt\_attr\_value\_changed\_ev::data\_length

> attribute data length

#### uint8\_t gatt\_attr\_value\_changed\_ev::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_ccc\_cfg\_changed\_ev Struct Reference

### Data Fields

  - uint16\_t ccc\_value

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the value of the Bluetooth LE client
characteristic configuration changed event which indicates whether the
characteristic notification or indication is enabled or disabled to the
client.

### Field Documentation

#### uint16\_t gatt\_ccc\_cfg\_changed\_ev::ccc\_value

> ccc (client characteristic configuration) value

#### uint8\_t gatt\_ccc\_cfg\_changed\_ev::uuid\_length

> UUID length

#### uint8\_t gatt\_ccc\_cfg\_changed\_ev::uuid\[SERVER\_MAX\_UUID\_LEN\]

> UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_cfg\_notify\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t enable

  - uint16\_t ccc\_handle

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE notify command.

### Field Documentation

#### uint8\_t gatt\_cfg\_notify\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_cfg\_notify\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gatt\_cfg\_notify\_cmd::enable

> enable characteristic notification
> 
> 0: disable
> 
> 1: enable

#### uint16\_t gatt\_cfg\_notify\_cmd::ccc\_handle

> characteristic configure change handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_cfg\_notify\_mult\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t cnt

  - uint16\_t attr\_id \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE notify multiple service command.

### Field Documentation

#### uint8\_t gatt\_cfg\_notify\_mult\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_cfg\_notify\_mult\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_cfg\_notify\_mult\_cmd::cnt

> attribute number

#### uint16\_t gatt\_cfg\_notify\_mult\_cmd::attr\_id\[\]

> attribute id list

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_change\_db\_cmd Struct Reference

### Data Fields

  - uint16\_t start\_handle

  - uint8\_t visibility

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE change DB (data base) command.

### Field Documentation

#### uint16\_t gatt\_change\_db\_cmd::start\_handle

> start handle UUID

#### uint8\_t gatt\_change\_db\_cmd::visibility

> visibility setting

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_char\_value Struct Reference

### Data Fields

  - uint16\_t handle

  - uint8\_t data\_len

  - uint8\_t data \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
characteristics.

### Field Documentation

#### uint16\_t gatt\_char\_value::handle

> characteristic handle

#### uint8\_t gatt\_char\_value::data\_len

> characteristic data length

#### uint8\_t gatt\_char\_value::data\[SERVER\_MAX\_UUID\_LEN\]

> characteristic data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## GATT\_CHARACTERISTIC Struct Reference

### Data Fields

  - uint16\_t characteristic\_handle

  - uint16\_t value\_handle

  - uint8\_t properties

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
characteristic data.

### Field Documentation

#### uint16\_t GATT\_CHARACTERISTIC::characteristic\_handle

> characteristic handle

#### uint16\_t GATT\_CHARACTERISTIC::value\_handle

> characteristic value handle

#### uint8\_t GATT\_CHARACTERISTIC::properties

> characteristic properties handle

#### uint8\_t GATT\_CHARACTERISTIC::uuid\_length

> characteristic UUID length

#### uint8\_t GATT\_CHARACTERISTIC::uuid\[SERVER\_MAX\_UUID\_LEN\]

> characteristic UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_characteristic Struct Reference

### Data Fields

  - uint16\_t characteristic\_handle

  - uint16\_t value\_handle

  - uint8\_t properties

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
characteristic data.

### Field Documentation

#### uint16\_t gatt\_characteristic::characteristic\_handle

> characteristic handle

#### uint16\_t gatt\_characteristic::value\_handle

> value handle

#### uint8\_t gatt\_characteristic::properties

> characteristic properties

#### uint8\_t gatt\_characteristic::uuid\_length

> characteristic UUID length

#### uint8\_t gatt\_characteristic::uuid\[SERVER\_MAX\_UUID\_LEN\]

> characteristic UUIDs

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_descriptor Struct Reference

### Data Fields

  - uint16\_t descriptor\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
descriptor data.

### Field Documentation

#### uint16\_t gatt\_descriptor::descriptor\_handle

> descriptor handle

#### uint8\_t gatt\_descriptor::uuid\_length

> descriptor UUID length

#### uint8\_t gatt\_descriptor::uuid\[SERVER\_MAX\_UUID\_LEN\]

> descriptor UUID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## GATT\_DESCRIPTOR Struct Reference

### Data Fields

  - uint16\_t descriptor\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
descriptor data.

### Field Documentation

#### uint16\_t GATT\_DESCRIPTOR::descriptor\_handle

> descriptor handle

#### uint8\_t GATT\_DESCRIPTOR::uuid\_length

> descriptor UUID length

#### uint8\_t GATT\_DESCRIPTOR::uuid\[SERVER\_MAX\_UUID\_LEN\]

> descriptor UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_all\_chrc\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE discovery characteristic command.

### Field Documentation

#### TypeHeader\_t gatt\_disc\_all\_chrc\_cmd::header

> Header of the type tlv

#### uint8\_t gatt\_disc\_all\_chrc\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_disc\_all\_chrc\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_disc\_all\_chrc\_cmd::start\_handle

> characteristic start handle

#### uint16\_t gatt\_disc\_all\_chrc\_cmd::end\_handle

> characteristic end handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_all\_desc\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE discovery descriptor command.

### Field Documentation

#### uint8\_t gatt\_disc\_all\_desc\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_disc\_all\_desc\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_disc\_all\_desc\_cmd::start\_handle

> descriptor start handle

#### uint16\_t gatt\_disc\_all\_desc\_cmd::end\_handle

> descriptor end handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_all\_desc\_rp Struct Reference

### Data Fields

  - uint8\_t descriptors\_count

  - gatt\_descriptor\_t descriptors \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discovery
descriptor command.

### Field Documentation

#### uint8\_t gatt\_disc\_all\_desc\_rp::descriptors\_count

> The number of discovered descriptors

#### gatt\_descriptor\_t gatt\_disc\_all\_desc\_rp::descriptors\[\]

> discovered descriptors

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_all\_prim\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE discovery primary service command.

### Field Documentation

#### uint8\_t gatt\_disc\_all\_prim\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_disc\_all\_prim\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_all\_prim\_rp Struct Reference

### Data Fields

  - uint8\_t services\_count

  - gatt\_service\_t services \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discovery
primary service command.

### Field Documentation

#### uint8\_t gatt\_disc\_all\_prim\_rp::services\_count

> discovered service number

#### gatt\_service\_t gatt\_disc\_all\_prim\_rp::services\[\]

> discovered service

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_chrc\_rp Struct Reference

### Data Fields

  - uint8\_t characteristics\_count

  - gatt\_characteristic\_t characteristics \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discovery
characteristic command.

### Field Documentation

#### uint8\_t gatt\_disc\_chrc\_rp::characteristics\_count

> discovered characteristic number

#### gatt\_characteristic\_t gatt\_disc\_chrc\_rp::characteristics\[\]

> discovered characteristic

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_chrc\_uuid\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE discovery characteristic UUID command.

### Field Documentation

#### TypeHeader\_t gatt\_disc\_chrc\_uuid\_cmd::header

> Header of the type tlv

#### uint8\_t gatt\_disc\_chrc\_uuid\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_disc\_chrc\_uuid\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_disc\_chrc\_uuid\_cmd::start\_handle

> characteristic start handle

#### uint16\_t gatt\_disc\_chrc\_uuid\_cmd::end\_handle

> characteristic end handle

#### uint8\_t gatt\_disc\_chrc\_uuid\_cmd::uuid\_length

> characteristic UUID length

#### uint8\_t gatt\_disc\_chrc\_uuid\_cmd::uuid\[\]

> characteristic UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_desc\_uuid\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE discovery descriptor UUID command.

### Field Documentation

#### TypeHeader\_t gatt\_disc\_desc\_uuid\_cmd::header

> Header of the type tlv

#### uint8\_t gatt\_disc\_desc\_uuid\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_disc\_desc\_uuid\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_disc\_desc\_uuid\_cmd::start\_handle

> descriptor start handle

#### uint16\_t gatt\_disc\_desc\_uuid\_cmd::end\_handle

> descriptor end handle

#### uint8\_t gatt\_disc\_desc\_uuid\_cmd::uuid\_length

> descriptor UUID length

#### uint8\_t gatt\_disc\_desc\_uuid\_cmd::uuid\[\]

> descriptor UUID values

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_prim\_rp Struct Reference

### Data Fields

  - uint8\_t services\_count

  - gatt\_service\_t services \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE discovery
primary UUID command.

### Field Documentation

#### uint8\_t gatt\_disc\_prim\_rp::services\_count

> The number of discovered services

#### gatt\_service\_t gatt\_disc\_prim\_rp::services\[\]

> discovered services

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_disc\_prim\_uuid\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t uuid\_length

  - uint8\_t uuid \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE discovery primary UUID command.

### Field Documentation

#### TypeHeader\_t gatt\_disc\_prim\_uuid\_cmd::header

> Header of the type tlv

#### uint8\_t gatt\_disc\_prim\_uuid\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_disc\_prim\_uuid\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gatt\_disc\_prim\_uuid\_cmd::uuid\_length

> UUID length

#### uint8\_t gatt\_disc\_prim\_uuid\_cmd::uuid\[\]

> UUID value list

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_eatt\_connect\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t num\_channels

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE gatt eatt connect command.

### Field Documentation

#### uint8\_t gatt\_eatt\_connect\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_eatt\_connect\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gatt\_eatt\_connect\_cmd::num\_channels

> channel number

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_exchange\_mtu\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE exchange maximum transmission unit command.

### Field Documentation

#### uint8\_t gatt\_exchange\_mtu\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_exchange\_mtu\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_find\_included\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE find included service command.

### Field Documentation

#### uint8\_t gatt\_find\_included\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_find\_included\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_find\_included\_cmd::start\_handle

> service start handle

#### uint16\_t gatt\_find\_included\_cmd::end\_handle

> service end handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_find\_included\_rp Struct Reference

### Data Fields

  - uint8\_t services\_count

  - gatt\_included\_t included \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE find
included service command.

### Field Documentation

#### uint8\_t gatt\_find\_included\_rp::services\_count

> The number of discovered services

#### gatt\_included\_t gatt\_find\_included\_rp::included\[\]

> discovered included service

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_get\_attribute\_value\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE get attribute value command.

### Field Documentation

#### uint8\_t gatt\_get\_attribute\_value\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_get\_attribute\_value\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_get\_attribute\_value\_cmd::handle

> attribute handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_get\_attribute\_value\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

  - uint16\_t value\_length

  - uint8\_t value \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE get
attribute value command.

### Field Documentation

#### uint8\_t gatt\_get\_attribute\_value\_rp::att\_response

> attribute response

#### uint16\_t gatt\_get\_attribute\_value\_rp::value\_length

> attribute value length

#### uint8\_t gatt\_get\_attribute\_value\_rp::value\[\]

> attribute value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_get\_attributes\_cmd Struct Reference

### Data Fields

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t type\_length

  - uint8\_t type \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE get attributes command.

### Field Documentation

#### uint16\_t gatt\_get\_attributes\_cmd::start\_handle

> attribute start handle

#### uint16\_t gatt\_get\_attributes\_cmd::end\_handle

> attribute end handle

#### uint8\_t gatt\_get\_attributes\_cmd::type\_length

> attribute type length

#### uint8\_t gatt\_get\_attributes\_cmd::type\[\]

> attribute type

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_get\_attributes\_rp Struct Reference

### Data Fields

  - uint8\_t attrs\_count

  - uint8\_t attrs \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE get
attributes command.

### Field Documentation

#### uint8\_t gatt\_get\_attributes\_rp::attrs\_count

> attribute count number

#### uint8\_t gatt\_get\_attributes\_rp::attrs\[\]

> attribute list

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_included Struct Reference

### Data Fields

  - uint16\_t included\_handle

  - struct gatt\_service service

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
service data.

### Field Documentation

#### uint16\_t gatt\_included::included\_handle

> service include UUID

#### struct gatt\_service gatt\_included::service

> service handle

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_ncp\_ble\_add\_service\_cmd Struct Reference

### Data Fields

  - uint8\_t svc\_length

  - uint8\_t svc \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE add service command.

### Field Documentation

#### uint8\_t gatt\_ncp\_ble\_add\_service\_cmd::svc\_length

> service length

#### uint8\_t gatt\_ncp\_ble\_add\_service\_cmd::svc\[\]

> service add list

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_ncp\_ble\_add\_service\_rp Struct Reference

### Data Fields

  - uint8\_t svc\_length

  - uint8\_t status \[\]

### Detailed Description

This structure contains the command response values of the Bluetooth LE
add service command.

### Field Documentation

#### uint8\_t gatt\_ncp\_ble\_add\_service\_rp::svc\_length

> service length

#### uint8\_t gatt\_ncp\_ble\_add\_service\_rp::status\[\]

> service status

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_ncp\_ble\_svc\_subscription\_ev Struct Reference

### Data Fields

  - uint8\_t svc\_id

  - uint8\_t status

### Detailed Description

This structure contains the value of the Bluetooth LE subscription event
which indicates the service characteristics are subscribed.

### Field Documentation

#### uint8\_t gatt\_ncp\_ble\_svc\_subscription\_ev::svc\_id

> service id

#### uint8\_t gatt\_ncp\_ble\_svc\_subscription\_ev::status

> subscription status

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_notification\_ev Struct Reference

### Data Fields

  - uint8\_t svc\_id

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t type

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the value of the Bluetooth LE GATT notification
event which notifies characteristic values to the client.

### Field Documentation

#### uint8\_t gatt\_notification\_ev::svc\_id

> service id

#### uint8\_t gatt\_notification\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_notification\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gatt\_notification\_ev::type

> attribute type

#### uint16\_t gatt\_notification\_ev::handle

> attribute handle UUID

#### uint16\_t gatt\_notification\_ev::data\_length

> attribute data length

#### uint8\_t gatt\_notification\_ev::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE read attribute command.

### Field Documentation

#### uint8\_t gatt\_read\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_read\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_read\_cmd::handle

> handle UUID

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_long\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

  - uint16\_t offset

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE read long attribute command.

### Field Documentation

#### uint8\_t gatt\_read\_long\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_read\_long\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_read\_long\_cmd::handle

> handle UUID

#### uint16\_t gatt\_read\_long\_cmd::offset

> handle UUID offset

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_long\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE read long
attribute command.

### Field Documentation

#### uint8\_t gatt\_read\_long\_rp::att\_response

> attribute response

#### uint16\_t gatt\_read\_long\_rp::data\_length

> data length

#### uint8\_t gatt\_read\_long\_rp::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_multiple\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint8\_t handles\_count

  - uint16\_t handles \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE read multiple attributes command.

### Field Documentation

#### uint8\_t gatt\_read\_multiple\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_read\_multiple\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint8\_t gatt\_read\_multiple\_cmd::handles\_count

> handle UUID number

#### uint16\_t gatt\_read\_multiple\_cmd::handles\[\]

> handle UUID list

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_multiple\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE read
multiple atrributes command.

### Field Documentation

#### uint8\_t gatt\_read\_multiple\_rp::att\_response

> attribute response

#### uint16\_t gatt\_read\_multiple\_rp::data\_length

> attribute data length

#### uint8\_t gatt\_read\_multiple\_rp::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE read
attribute command.

### Field Documentation

#### uint8\_t gatt\_read\_rp::att\_response

> attribute response

#### uint16\_t gatt\_read\_rp::data\_length

> data length

#### uint8\_t gatt\_read\_rp::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_uuid\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE read UUID
command.

### Field Documentation

#### uint8\_t gatt\_read\_uuid\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_read\_uuid\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_read\_uuid\_cmd::start\_handle

> UUID start handle

#### uint16\_t gatt\_read\_uuid\_cmd::end\_handle

> UUID end handle

#### uint8\_t gatt\_read\_uuid\_cmd::uuid\_length

> UUID length

#### uint8\_t gatt\_read\_uuid\_cmd::uuid\[\]

> UUID data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_read\_uuid\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

  - uint8\_t values\_count

  - gatt\_char\_value\_t values \[SERVER\_MAX\_ATTRIBUTES\]

### Detailed Description

This structure is returned as an output from the Bluetooth LE read UUID
command.

### Field Documentation

#### uint8\_t gatt\_read\_uuid\_rp::att\_response

> attribute response

#### uint8\_t gatt\_read\_uuid\_rp::values\_count

> attribute value number

#### gatt\_char\_value\_t gatt\_read\_uuid\_rp::values\[SERVER\_MAX\_ATTRIBUTES\]

> characteristic value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_reliable\_write\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

  - uint16\_t offset

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE write reliable attribute command.

### Field Documentation

#### uint8\_t gatt\_reliable\_write\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_reliable\_write\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_reliable\_write\_cmd::handle

> handle UUID

#### uint16\_t gatt\_reliable\_write\_cmd::offset

> handle offset

#### uint16\_t gatt\_reliable\_write\_cmd::data\_length

> attribute data length

#### uint8\_t gatt\_reliable\_write\_cmd::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_reliable\_write\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

### Detailed Description

This structure is returned as an output from the Bluetooth LE write
reliable attribute command.

### Field Documentation

#### uint8\_t gatt\_reliable\_write\_rp::att\_response

> attribute response

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_service Struct Reference

### Data Fields

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
service characteristic data.

### Field Documentation

#### uint16\_t gatt\_service::start\_handle

> service start UUID

#### uint16\_t gatt\_service::end\_handle

> service end UUID

#### uint8\_t gatt\_service::uuid\_length

> UUID length

#### uint8\_t gatt\_service::uuid\[SERVER\_MAX\_UUID\_LEN\]

> UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## GATT\_SERVICE Struct Reference

### Data Fields

  - uint16\_t start\_handle

  - uint16\_t end\_handle

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

This structure contains the configuration values of the Bluetooth LE
service data.

### Field Documentation

#### uint16\_t GATT\_SERVICE::start\_handle

> service start handle

#### uint16\_t GATT\_SERVICE::end\_handle

> service end handle

#### uint8\_t GATT\_SERVICE::uuid\_length

> service UUID length

#### uint8\_t GATT\_SERVICE::uuid\[SERVER\_MAX\_UUID\_LEN\]

> service UUID value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_set\_enc\_key\_size\_cmd Struct Reference

### Data Fields

  - uint16\_t attr\_id

  - uint8\_t key\_size

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set encryption key size command.

### Field Documentation

#### uint16\_t gatt\_set\_enc\_key\_size\_cmd::attr\_id

> attribute id

#### uint8\_t gatt\_set\_enc\_key\_size\_cmd::key\_size

> encryption key size

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_set\_value\_cmd Struct Reference

### Data Fields

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

  - uint16\_t len

  - uint8\_t value \[512\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE set value command.

### Field Documentation

#### uint8\_t gatt\_set\_value\_cmd::uuid\_length

> characteristic UUID length

#### uint8\_t gatt\_set\_value\_cmd::uuid\[SERVER\_MAX\_UUID\_LEN\]

> characteristic UUID

#### uint16\_t gatt\_set\_value\_cmd::len

> value length

#### uint8\_t gatt\_set\_value\_cmd::value\[512\]

> characteristic value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_signed\_write\_without\_rsp\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE signed write without response command.

### Field Documentation

#### uint8\_t gatt\_signed\_write\_without\_rsp\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_signed\_write\_without\_rsp\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_signed\_write\_without\_rsp\_cmd::handle

> handle UUID

#### uint16\_t gatt\_signed\_write\_without\_rsp\_cmd::data\_length

> attribute data length

#### uint8\_t gatt\_signed\_write\_without\_rsp\_cmd::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_start\_server\_rp Struct Reference

### Data Fields

  - uint16\_t db\_attr\_off

  - uint8\_t db\_attr\_cnt

### Detailed Description

This structure is returned as an output from the Bluetooth LE start
server command.

### Field Documentation

#### uint16\_t gatt\_start\_server\_rp::db\_attr\_off

> database attribute offset

#### uint8\_t gatt\_start\_server\_rp::db\_attr\_cnt

> database attribute count

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_start\_service\_cmd Struct Reference

### Data Fields

  - TypeHeader\_t header

  - uint8\_t started

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE start service command.

### Field Documentation

#### TypeHeader\_t gatt\_start\_service\_cmd::header

> Header of the type tlv

#### uint8\_t gatt\_start\_service\_cmd::started

> service started status
> 
> 0: unstart
> 
> 1: start

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_write\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE write attribute command.

### Field Documentation

#### uint8\_t gatt\_write\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_write\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_write\_cmd::handle

> handle UUID

#### uint16\_t gatt\_write\_cmd::data\_length

> attribute data length

#### uint8\_t gatt\_write\_cmd::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_write\_long\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

  - uint16\_t offset

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE write long attribute command.

### Field Documentation

#### uint8\_t gatt\_write\_long\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_write\_long\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_write\_long\_cmd::handle

> handle UUID

#### uint16\_t gatt\_write\_long\_cmd::offset

> handle offset

#### uint16\_t gatt\_write\_long\_cmd::data\_length

> attribute data length

#### uint8\_t gatt\_write\_long\_cmd::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_write\_long\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

### Detailed Description

This structure is returned as an output from the Bluetooth LE write long
attribute command.

### Field Documentation

#### uint8\_t gatt\_write\_long\_rp::att\_response

> attribute response

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_write\_rp Struct Reference

### Data Fields

  - uint8\_t att\_response

### Detailed Description

This structure is returned as an output from the Bluetooth LE write
attribute command.

### Field Documentation

#### uint8\_t gatt\_write\_rp::att\_response

> attribute response

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## gatt\_write\_without\_rsp\_cmd Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t handle

  - uint16\_t data\_length

  - uint8\_t data \[\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE write without response command.

### Field Documentation

#### uint8\_t gatt\_write\_without\_rsp\_cmd::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t gatt\_write\_without\_rsp\_cmd::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t gatt\_write\_without\_rsp\_cmd::handle

> handle UUID

#### uint16\_t gatt\_write\_without\_rsp\_cmd::data\_length

> attribute data length

#### uint8\_t gatt\_write\_without\_rsp\_cmd::data\[\]

> attribute data

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## host\_gatt\_attr Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t properties

  - uint16\_t permissions

  - uint8\_t uuid\_length

  - uint8\_t uuid \[SERVER\_MAX\_UUID\_LEN\]

### Detailed Description

NCP Bluetooth LE host gatt attribute type structure

### Field Documentation

#### uint8\_t host\_gatt\_attr::type

> NCP Bluetooth LE TLV type

#### uint8\_t host\_gatt\_attr::properties

> attribute properties

#### uint16\_t host\_gatt\_attr::permissions

> attribute permissions

#### uint8\_t host\_gatt\_attr::uuid\_length

> service uuid length

#### uint8\_t host\_gatt\_attr::uuid\[SERVER\_MAX\_UUID\_LEN\]

> service uuid

#### The documentation for this struct was generated from the following file:

  - service.h

#### 

## hr\_measurement Struct Reference

### Data Fields

  - uint8\_t sensor

  - uint8\_t rate

### Detailed Description

Heart Rate format

### Field Documentation

#### uint8\_t hr\_measurement::sensor

> sensor id

#### uint8\_t hr\_measurement::rate

> heart rate

#### The documentation for this struct was generated from the following file:

  - ht.h

#### 

## icmp\_echo\_hdr Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t code

  - uint16\_t chksum

  - uint16\_t id

  - uint16\_t seqno

### Detailed Description

This structure is used for ICMP (internet control message protocol) echo
header configuration.

### Field Documentation

#### uint8\_t icmp\_echo\_hdr::type

> Indicates the type of ICMP packet.

#### uint8\_t icmp\_echo\_hdr::code

> Identifies the code of the corresponding ICMP packet. Together with
> the Type field, it identifies the detailed type of an ICMP packet.

#### uint16\_t icmp\_echo\_hdr::chksum

> This checksum checksums the entire ICMP datagram, including the data
> portion of the ICMP packet, to check whether there is an error in the
> transmission of the packet.

#### uint16\_t icmp\_echo\_hdr::id

> This field is used to identify the ICMP process, but is only
> applicable to echoing requests and replies to ICMP packets, and has a
> value of 0 for ICMP packets whose destinations are unreachable and
> ICMP packets that have timed out.

#### uint16\_t icmp\_echo\_hdr::seqno

> Packet sequence number, which is used to mark the order of ICMP
> packets.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## ip\_hdr Struct Reference

### Data Fields

  - uint8\_t \_v\_hl

  - uint8\_t \_tos

  - uint16\_t \_len

  - uint16\_t \_id

  - uint16\_t \_offset

  - uint8\_t \_ttl

  - uint8\_t \_proto

  - uint16\_t \_chksum

  - in\_addr\_t src

  - in\_addr\_t dest

### Detailed Description

This structure is used for IP header configuration.

### Field Documentation

#### uint8\_t ip\_hdr::\_v\_hl

> Version / header length

#### uint8\_t ip\_hdr::\_tos

> Type of service.

#### uint16\_t ip\_hdr::\_len

> Total length.

#### uint16\_t ip\_hdr::\_id

> Identification.

#### uint16\_t ip\_hdr::\_offset

> Fragment offset field

#### uint8\_t ip\_hdr::\_ttl

> Time to live.

#### uint8\_t ip\_hdr::\_proto

> Protocol type.

#### uint16\_t ip\_hdr::\_chksum

> Checksum.

#### in\_addr\_t ip\_hdr::src

> Source IP address.

#### in\_addr\_t ip\_hdr::dest

> Destination IP address.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## iperf\_msg\_t Struct Reference

### Data Fields

  - int16\_t status \[2\]

  - uint32\_t count

  - uint32\_t timeout

  - uint32\_t handle

  - uint32\_t port

  - uint16\_t per\_size

  - char ip\_addr \[IP\_ADDR\_LEN\]

  - iperf\_set\_t iperf\_set

### Detailed Description

This structure is used for iperf message configuration.

### Field Documentation

#### int16\_t iperf\_msg\_t::status\[2\]

> Iperf status.
> 
> status\[0\] is for TX,
> 
> status\[1\] is for RX.

#### uint32\_t iperf\_msg\_t::count

> Count of iperf package.

#### uint32\_t iperf\_msg\_t::timeout

> Timeout period.

#### uint32\_t iperf\_msg\_t::handle

> Socket handle index number.

#### uint32\_t iperf\_msg\_t::port

> Peer port number.

#### uint16\_t iperf\_msg\_t::per\_size

> Size of per UDP package.

#### char iperf\_msg\_t::ip\_addr\[IP\_ADDR\_LEN\]

> IP address.

#### iperf\_set\_t iperf\_msg\_t::iperf\_set

> Iperf configuration.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## iperf\_set\_t Struct Reference

### Data Fields

  - uint32\_t iperf\_type

  - uint32\_t iperf\_count

  - uint32\_t iperf\_udp\_rate

  - uint32\_t iperf\_udp\_time

### Detailed Description

This structure is used for iperf configuration.

### Field Documentation

#### uint32\_t iperf\_set\_t::iperf\_type

> Iperf type,
> 
> 0: TCP TX,
> 
> 1: TCP RX,
> 
> 2: UDP TX,
> 
> 3: UDP RX.

#### uint32\_t iperf\_set\_t::iperf\_count

> Iperf package count.

#### uint32\_t iperf\_set\_t::iperf\_udp\_rate

> UDP rate.

#### uint32\_t iperf\_set\_t::iperf\_udp\_time

> UDP time.

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## l2cap\_connect\_cmd\_tag Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

  - uint8\_t sec

  - uint8\_t sec\_flag

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE L2CAP connection command.

### Field Documentation

#### uint8\_t l2cap\_connect\_cmd\_tag::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t l2cap\_connect\_cmd\_tag::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t l2cap\_connect\_cmd\_tag::psm

> Protocol/Service Multiplexer

#### uint8\_t l2cap\_connect\_cmd\_tag::sec

> connection security level
> 
> 0x01: No encryption and no authentication
> 
> 0x02: Encryption and no authentication (no MITM)
> 
> 0x03: Encryption and authentication (MITM)

#### uint8\_t l2cap\_connect\_cmd\_tag::sec\_flag

> Option to use custom security level or to use system default security
> level
> 
> 0: to use system default security level
> 
> 1: to use customized security level

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_connect\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

### Detailed Description

This structure contains the value of the Bluetooth LE L2CAP (Logical
Link Control and Adaptation Layer Protocol) connection complete event
which indicates an L2CAP connection is established.

### Field Documentation

#### uint8\_t l2cap\_connect\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t l2cap\_connect\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t l2cap\_connect\_ev::psm

> PSM (Protocol/Service Multiplexer) value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_connect\_rp\_tag Struct Reference

### Data Fields

  - uint8\_t num

  - uint8\_t chan\_id \[\]

### Detailed Description

This structure is returned as an output of the Bluetooth LE L2CAP
connection command.

### Field Documentation

#### uint8\_t l2cap\_connect\_rp\_tag::num

> receive packet number

#### uint8\_t l2cap\_connect\_rp\_tag::chan\_id\[\]

> receive channel id

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_disconnect\_cmd\_tag Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE L2CAP disconnection command.

### Field Documentation

#### uint8\_t l2cap\_disconnect\_cmd\_tag::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t l2cap\_disconnect\_cmd\_tag::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_disconnect\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

### Detailed Description

This structure contains the value of the Bluetooth LE L2CAP (Logical
Link Control and Adaptation Layer Protocol) disconnection complete event
which indicates an L2CAP connection is terminated.

### Field Documentation

#### uint8\_t l2cap\_disconnect\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t l2cap\_disconnect\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t l2cap\_disconnect\_ev::psm

> PSM (Protocol/Service Multiplexer) value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_metrics\_cmd\_tag Struct Reference

### Data Fields

  - bool metrics\_flag

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE l2cap metrics command.

### Field Documentation

#### bool l2cap\_metrics\_cmd\_tag::metrics\_flag

> option to enable calculating metrics of l2cap rate
> 
> 0: disable calculating metrics of l2cap rate
> 
> 1: enable calculating metrics of l2cap rate

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_receive\_ev Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t psm

  - uint8\_t len

  - uint8\_t data \[256\]

### Detailed Description

This structure contains the value of the Bluetooth LE L2CAP (Logical
Link Control and Adaptation Layer Protocol) receive packet event which
notifies the receive data over L2CAP channel to the client.

### Field Documentation

#### uint8\_t l2cap\_receive\_ev::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t l2cap\_receive\_ev::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t l2cap\_receive\_ev::psm

> PSM (Protocol/Service Multiplexer) value

#### uint8\_t l2cap\_receive\_ev::len

> receive data length

#### uint8\_t l2cap\_receive\_ev::data\[256\]

> receive data value

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_recv\_cmd\_tag Struct Reference

### Data Fields

  - uint32\_t l2cap\_recv\_delay\_ms

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE l2cap receive command.

### Field Documentation

#### uint32\_t l2cap\_recv\_cmd\_tag::l2cap\_recv\_delay\_ms

> receive delay time in millisecond

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_register\_psm\_cmd\_tag Struct Reference

### Data Fields

  - uint16\_t psm

  - uint8\_t sec\_level

  - uint8\_t sec\_flag

  - uint8\_t policy

  - uint8\_t policy\_flag

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE l2cap register PSM (Protocol/Service Multiplexer)
command.

### Field Documentation

#### uint16\_t l2cap\_register\_psm\_cmd\_tag::psm

> protocol service multiplexor

#### uint8\_t l2cap\_register\_psm\_cmd\_tag::sec\_level

> connection security level
> 
> 0x01: No encryption and no authentication
> 
> 0x02: Encryption and no authentication (no MITM)
> 
> 0x03: Encryption and authentication (MITM)

#### uint8\_t l2cap\_register\_psm\_cmd\_tag::sec\_flag

> Option to use custom security level or to use system default security
> level
> 
> 0: use system default security level
> 
> 1: use customized security level

#### uint8\_t l2cap\_register\_psm\_cmd\_tag::policy

> allowlist

#### uint8\_t l2cap\_register\_psm\_cmd\_tag::policy\_flag

> option to use allowlist
> 
> 0: do not use allowlist
> 
> 1: use allowlist

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## l2cap\_send\_data\_cmd\_tag Struct Reference

### Data Fields

  - uint8\_t address\_type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

  - uint16\_t times

### Detailed Description

This structure contains the configuration values that are used as input
to the Bluetooth LE L2CAP send data command.

### Field Documentation

#### uint8\_t l2cap\_send\_data\_cmd\_tag::address\_type

> Bluetooth LE address type
> 
> 0: public
> 
> 1: random

#### uint8\_t l2cap\_send\_data\_cmd\_tag::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth LE address

#### uint16\_t l2cap\_send\_data\_cmd\_tag::times

> the number of times a loop is sent

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## le\_addr\_t Struct Reference

### Data Fields

  - uint8\_t type

  - uint8\_t address \[NCP\_BLE\_ADDR\_LENGTH\]

### Detailed Description

This structure contains the configuration values of Bluetooth LE address
type.

### Field Documentation

#### uint8\_t le\_addr\_t::type

> Types of Bluetooth LE device addresses:
> 
> Public Address : 0x00
> 
> Random Address : 0x01
> 
> Public Identity Address : 0x02
> 
> Random Identity Address : 0x03

#### uint8\_t le\_addr\_t::address\[NCP\_BLE\_ADDR\_LENGTH\]

> Bluetooth device Address

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_ble.h

#### 

## temp\_measurement Struct Reference

### Data Fields

  - uint8\_t flags

  - uint8\_t temperature \[4\]

  - uint8\_t type

### Detailed Description

Temperature measurement format

### Field Documentation

#### uint8\_t temp\_measurement::flags

> temperature type flag
> 
> 0: Fahrenheit
> 
> 1: Celsius

#### uint8\_t temp\_measurement::temperature\[4\]

> temperature

#### uint8\_t temp\_measurement::type

> Possible temperature sensor locations
> 
> 0: hts\_no\_temp\_type
> 
> 1: hts\_armpit
> 
> 2: hts\_body
> 
> 3: hts\_ear
> 
> 4: hts\_finger
> 
> 5: hts\_gastroInt
> 
> 6: hts\_mouth
> 
> 7: hts\_rectum
> 
> 8: hts\_toe
> 
> 9: hts\_tympanum

#### The documentation for this struct was generated from the following file:

  - ht.h

#### 

## wifi\_sta\_info\_t Struct Reference

### Data Fields

  - uint8\_t mac \[MLAN\_MAC\_ADDR\_LENGTH\]

  - uint8\_t power\_mgmt\_status

  - char rssi

### Detailed Description

This structure is used for station configuration.

### Field Documentation

#### uint8\_t wifi\_sta\_info\_t::mac\[MLAN\_MAC\_ADDR\_LENGTH\]

> MAC address buffer.

#### uint8\_t wifi\_sta\_info\_t::power\_mgmt\_status

> Power management status,
> 
> 0: active (not in power save),
> 
> 1: in power save status.

#### char wifi\_sta\_info\_t::rssi

> Value of RSSI: dBm

#### The documentation for this struct was generated from the following file:

  - ncp\_host\_command\_wifi.h

#### 

## WLAN\_RESET\_ParaSet Struct Reference

### Data Fields

  - uint8\_t option

### Detailed Description

This structure is used for Wi-Fi reset option.

### Field Documentation

#### uint8\_t WLAN\_RESET\_ParaSet::option

> Wi-Fi reset option, there are 3 modes,
> 
> 0: disable Wi-Fi;
> 
> 1: enable Wi-Fi;
> 
> 2: reset Wi-Fi.

#### The documentation for this struct was generated from the following file:

  - ncp\_cmd\_wifi.h

# File Documentation

## bas.h File Reference

Battery Service Profile definitions.

### Function Documentation

#### void bas\_init (void )

> Init BAS Service

#### void peripheral\_bas\_event\_put (osa\_event\_flags\_t *flag*)

> Count binary semaphore to wait for BAS write characteristic response
> event

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flag</em></td>
<td>flag to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void peripheral\_bas\_start (void )

> Start peripheral BAS Service

#### void peripheral\_bas\_indicate (uint8\_t *value*)

> Indicate BAS characteristic value change event

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>value</em></td>
<td>bas value</td>
</tr>
</tbody>
</table>

##### Returns

> void

### Macro Documentation

#### \#define BAS\_EVENT\_WRITE\_CHRA\_RSP  0x01

> BAS write characteristic response event ID

#### \#define UUID\_BAS  0x180f

> Battery Service UUID value

#### \#define UUID\_BAS\_BATTERY\_LEVEL  0x2a19

> BAS Characteristic Battery Level UUID value

#### 

## hr.h File Reference

Health Rate Profile definitions.

### Function Documentation

#### void central\_hrc\_start (void )

> Start Central HRC Service

#### void hrc\_init (void )

> Init HRC Service

#### void hrs\_init (void )

> Init HRS Service

#### void peripheral\_hrs\_event\_put (osa\_event\_flags\_t *flag*)

> Count binary semaphore to wait for HRS write characteristic response
> event

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flag</em></td>
<td>flag to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void.

#### void peripheral\_hrs\_start (void )

> Start Peripheral HRS Service

#### void peripheral\_hrs\_indicate (uint8\_t *value*)

> Indicate HRS characteristic value change event

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>value</em></td>
<td>hrs value</td>
</tr>
</tbody>
</table>

##### Returns

> void

### Macro Documentation

#### \#define HRC\_EVENT\_DEVICE\_FOUND  0x01

> HRC device found event ID

#### \#define HRC\_EVENT\_CONNECTED  0x02

> HRC connected event ID

#### \#define HRC\_EVENT\_GET\_PRIMARY\_SERVICE  0x03

> HRC get primary service event ID

#### \#define HRC\_EVENT\_GET\_CHARACTERISTICS  0x04

> HRC get characteristic event ID

#### \#define HRC\_EVENT\_GET\_CCC  0x05

> HRC get characteristic configuration changed event ID

#### \#define HRS\_EVENT\_WRITE\_CHRA\_RSP  0x01

> HRC write characteristic response event ID

#### \#define UUID\_HRS  0x180d

> Heart Rate Service UUID value

#### \#define UUID\_HRS\_MEASUREMENT  0x2a37

> HRS Characteristic Measurement Interval UUID value

#### \#define UUID\_HRS\_BODY\_SENSOR  0x2a38

> HRS Characteristic Body Sensor Location

#### \#define UUID\_HRS\_CONTROL\_POINT  0x2a39

> HRS Characteristic Control Point UUID value

#### 

## ht.h File Reference

Health Thermometer Profile definitions.

### Function Documentation

#### void central\_htc\_start (void )

> Start central HTC (Health Thermometer) service

#### void htc\_init (void )

> Init HTC Service

#### void hts\_init (void )

> Init HTS Service

#### void peripheral\_hts\_event\_put (osa\_event\_flags\_t *flag*)

> Count binary semaphore to wait for HTS write characteristic response
> event

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flag</em></td>
<td>flag to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void peripheral\_hts\_start (void )

> Start Peripheral HTS Service

#### void peripheral\_hts\_indicate (uint8\_t *value*)

> Indicate HTS characteristic value change event

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>value</em></td>
<td>hts value</td>
</tr>
</tbody>
</table>

##### Returns

> void

### Macro Documentation

#### \#define HTC\_EVENT\_DEVICE\_FOUND  0x01

> HTC device found event ID

#### \#define HTC\_EVENT\_CONNECTED  0x02

> HTC connected event ID

#### \#define HTC\_EVENT\_GET\_PRIMARY\_SERVICE  0x03

> HTC get primary service event ID

#### \#define HTC\_EVENT\_GET\_CHARACTERISTICS  0x04

> HTC get characteristic ID

#### \#define HTC\_EVENT\_GET\_CCC  0x05

> HTC get characteristic configuration changed event ID

#### \#define HTS\_EVENT\_WRITE\_CHRA\_RSP  0x01

> HTC write characteristic response event ID

#### \#define UUID\_GATT\_PRIMARY  0x2800

> GATT Primary Service UUID

#### \#define UUID\_HTS  0x1809

> Health Thermometer Service UUID

#### \#define UUID\_HTS\_MEASUREMENT  0x2a1c

> HTS Characteristic Measurement Value UUID

#### \#define UUID\_GATT\_CCC  0x2902

> GATT Client Characteristic Configuration UUID

#### \#define BT\_GATT\_CCC\_NOTIFY  0x0001

> Client Characteristic Configuration Values Client Characteristic
> Configuration Notification.
> 
> If set, changes to Characteristic Value are notified.

#### \#define BT\_GATT\_CCC\_INDICATE  0x0002

> Client Characteristic Configuration Indication.
> 
> If set, changes to Characteristic Value are indicated.

#### \#define hts\_unit\_celsius\_c  0x00U

> HTS flag values

#### 

## ncp\_cmd\_ble.h File Reference

NCP Bluetooth LE command and structure definitions.

### Macro Documentation

#### \#define NCP\_CMD\_BLE\_CORE  0x00000000

> NCP Bluetooth LE subclass type NCP Bluetooth LE subclass type for core
> command

#### \#define NCP\_CMD\_BLE\_GAP  0x00100000

> NCP Bluetooth LE subclass type for gap command

#### \#define NCP\_CMD\_BLE\_GATT  0x00200000

> NCP Bluetooth LE subclass type for gatt command

#### \#define NCP\_CMD\_BLE\_L2CAP  0x00300000

> NCP Bluetooth LE subclass type for l2cap command

#### \#define NCP\_CMD\_BLE\_POWERMGMT  0x00400000

> NCP Bluetooth LE subclass type for power management command

#### \#define NCP\_CMD\_BLE\_VENDOR  0x00500000

> NCP Bluetooth LE subclass type for vendor command

#### \#define NCP\_CMD\_BLE\_OTHER  0x00600000

> NCP Bluetooth LE subclass type for other command

#### \#define NCP\_CMD\_BLE\_EVENT  0x00f00000

> NCP Bluetooth LE subclass type for event

#### \#define NCP\_BLE\_MTU  1024

> The max size of the NCP Bluetooth LE max transmission unit

#### \#define NCP\_CMD\_BLE\_INVALID\_CMD  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_CORE | NCP\_MSG\_TYPE\_CMD | 0x0000000a)

> NCP Command/Response definitions Bluetooth LE invalid command ID

#### \#define NCP\_RSP\_BLE\_CORE\_SUPPORT\_CMD  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_CORE | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Bluetooth LE Core support command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_SET\_DATA\_LEN  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x00000020)

> Bluetooth LE GAP set data length command ID

#### \#define NCP\_RSP\_BLE\_GAP\_SET\_DATA\_LEN  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x00000020)

> Bluetooth LE GAP set data length command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_SET\_PHY  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000001f)

> Bluetooth LE GAP set phy command ID

#### \#define NCP\_RSP\_BLE\_GAP\_SET\_PHY  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000001f)

> Bluetooth LE GAP set phy command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_SET\_ADV\_DATA  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000001e)

> Bluetooth LE GAP set advertising data command ID

#### \#define NCP\_RSP\_BLE\_GAP\_SET\_ADV\_DATA  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000001e)

> Bluetooth LE GAP set advertising data command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_SET\_SCAN\_PARAM  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000001d)

> Bluetooth LE GAP set scan parameter command ID

#### \#define NCP\_RSP\_BLE\_GAP\_SET\_SCAN\_PARAM  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000001d)

> Bluetooth LE GAP set scan parameter command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_START\_ADV  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000000a)

> Bluetooth LE GAP start advertising command ID

#### \#define NCP\_RSP\_BLE\_GAP\_START\_ADV  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000000a)

> Bluetooth LE GAP start advertising command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_STOP\_ADV  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000000b)

> Bluetooth LE GAP stop advertising command ID

#### \#define NCP\_RSP\_BLE\_GAP\_STOP\_ADV  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000000b)

> Bluetooth LE GAP stop advertising command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_START\_SCAN  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000000c)

> Bluetooth LE GAP start scan command ID

#### \#define NCP\_RSP\_BLE\_GAP\_START\_SCAN  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000000c)

> Bluetooth LE GAP start scan command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_STOP\_SCAN  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000000d)

> Bluetooth LE GAP stop scan command ID

#### \#define NCP\_RSP\_BLE\_GAP\_STOP\_SCAN  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000000d)

> Bluetooth LE GAP start scan command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_CONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000000e)

> Bluetooth LE GAP connect command ID

#### \#define NCP\_RSP\_BLE\_GAP\_CONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000000e)

> Bluetooth LE GAP connect command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_DISCONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000000f)

> Bluetooth LE GAP disconnect command ID

#### \#define NCP\_RSP\_BLE\_GAP\_DISCONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000000f)

> Bluetooth LE GAP connect command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_CONN\_PARAM\_UPDATE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x00000016)

> Bluetooth LE GAP connection parameter update command ID

#### \#define NCP\_RSP\_BLE\_GAP\_CONN\_PARAM\_UPDATE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x00000016)

> Bluetooth LE GAP connection parameter update command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_SET\_FILTER\_LIST  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x0000001c)

> Bluetooth LE GAP set filter list command ID

#### \#define NCP\_RSP\_BLE\_GAP\_SET\_FILTER\_LIST  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x0000001c)

> Bluetooth LE GAP set filter list command response ID

#### \#define NCP\_CMD\_BLE\_GAP\_PAIR  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_CMD | 0x00000011)

> Bluetooth LE GAP pair command ID

#### \#define NCP\_RSP\_BLE\_GAP\_PAIR  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GAP | NCP\_MSG\_TYPE\_RESP | 0x00000011)

> Bluetooth LE GAP pair command response ID

#### \#define NCP\_CMD\_BLE\_HOST\_SERVICE\_ADD  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | 0x00000002)

> Bluetooth LE gatt add host service attribute command ID

#### \#define NCP\_RSP\_BLE\_HOST\_SERVICE\_ADD  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Bluetooth LE gatt add host service attribute command response ID

#### \#define NCP\_CMD\_BLE\_HOST\_SERVICE\_DISC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | 0x00000003)

> Bluetooth LE gatt discovery primary service/characteristic/descriptor
> command ID

#### \#define NCP\_RSP\_BLE\_HOST\_SERVICE\_DISC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Bluetooth LE gatt discovery primary service/characteristic/descriptor
> command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_SET\_VALUE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | 0x00000006)

> Bluetooth LE gatt set characteristic/descriptor value command ID

#### \#define NCP\_RSP\_BLE\_GATT\_SET\_VALUE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Bluetooth LE gatt set characteristic/descriptor value command response
> ID

#### \#define NCP\_CMD\_BLE\_GATT\_START\_SERVICE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | 0x00000007)

> Bluetooth LE gatt start service command ID

#### \#define NCP\_RSP\_BLE\_GATT\_START\_SERVICE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> Bluetooth LE gatt start service command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_DISC\_PRIM  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | GATT\_DISC\_PRIM\_UUID)

> Bluetooth LE gatt discovery primary service command ID

#### \#define NCP\_RSP\_BLE\_GATT\_DISC\_PRIM  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | GATT\_DISC\_PRIM\_UUID)

> Bluetooth LE gatt discovery primary service command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_DISC\_CHRC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | GATT\_DISC\_CHRC\_UUID)

> Bluetooth LE gatt discovery characteristic command ID

#### \#define NCP\_RSP\_BLE\_GATT\_DISC\_CHRC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | GATT\_DISC\_CHRC\_UUID)

> Bluetooth LE gatt discovery characteristic command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_READ  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | 0x00000011)

> Bluetooth LE gatt read characteristic/descriptor command ID

#### \#define NCP\_RSP\_BLE\_GATT\_READ  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | 0x00000011)

> Bluetooth LE gatt read characteristic/descriptor command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_CFG\_NOTIFY  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | GATT\_CFG\_NOTIFY)

> Bluetooth LE gatt config service notify characteristic value command
> ID

#### \#define NCP\_RSP\_BLE\_GATT\_CFG\_NOTIFY  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | GATT\_CFG\_NOTIFY)

> Bluetooth LE gatt config service notify characteristic value command
> response ID

#### \#define NCP\_CMD\_BLE\_GATT\_CFG\_INDICATE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | GATT\_CFG\_INDICATE)

> Bluetooth LE gatt config service indicate characteristic value command
> ID

#### \#define NCP\_RSP\_BLE\_GATT\_CFG\_INDICATE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | GATT\_CFG\_INDICATE)

> Bluetooth LE gatt config service indicate characteristic value command
> response ID

#### \#define NCP\_CMD\_BLE\_GATT\_WRITE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | GATT\_WRITE)

> Bluetooth LE gatt write characteristic/descriptor command ID

#### \#define NCP\_RSP\_BLE\_GATT\_WRITE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | GATT\_WRITE)

> Bluetooth LE gatt write characteristic/descriptor command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_REGISTER\_SERVICE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | 0x00000020)

> Bluetooth LE gatt register service command ID

#### \#define NCP\_RSP\_BLE\_GATT\_REGISTER\_SERVICE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | 0x00000020)

> Bluetooth LE gatt register service command response ID

#### \#define NCP\_CMD\_BLE\_GATT\_DESC\_CHRC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_CMD | GATT\_DISC\_DESC\_UUID)

> Bluetooth LE gatt discovery descriptor command ID

#### \#define NCP\_RSP\_BLE\_GATT\_DESC\_CHRC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_GATT | NCP\_MSG\_TYPE\_RESP | GATT\_DISC\_DESC\_UUID)

> Bluetooth LE gatt discovery descriptor command response ID

#### \#define NCP\_CMD\_BLE\_L2CAP\_CONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_CMD | 0x00000002)

> Bluetooth LE l2cap connect command ID

#### \#define NCP\_RSP\_BLE\_L2CAP\_CONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Bluetooth LE l2cap connect command response ID

#### \#define NCP\_CMD\_BLE\_L2CAP\_DISCONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_CMD | 0x00000003)

> Bluetooth LE l2cap disconnect command ID

#### \#define NCP\_RSP\_BLE\_L2CAP\_DISCONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Bluetooth LE l2cap disconnect command response ID

#### \#define NCP\_CMD\_BLE\_L2CAP\_SEND  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_CMD | 0x00000004)

> Bluetooth LE l2cap send command ID

#### \#define NCP\_RSP\_BLE\_L2CAP\_SEND  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Bluetooth LE l2cap send command response ID

#### \#define NCP\_CMD\_BLE\_L2CAP\_REGISTER  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_CMD | 0x0000000a)

> Bluetooth LE l2cap register command ID

#### \#define NCP\_RSP\_BLE\_L2CAP\_REGISTER  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_RESP | 0x0000000a)

> Bluetooth LE l2cap register command response ID

#### \#define NCP\_CMD\_BLE\_L2CAP\_METRICS  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_CMD | 0x0000000b)

> Bluetooth LE l2cap metrics command ID

#### \#define NCP\_RSP\_BLE\_L2CAP\_METRICS  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_RESP | 0x0000000b)

> Bluetooth LE l2cap metrics command response ID

#### \#define NCP\_CMD\_BLE\_L2CAP\_RECEIVE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_CMD | 0x0000000c)

> Bluetooth LE l2cap receive command ID

#### \#define NCP\_RSP\_BLE\_L2CAP\_RECEIVE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_L2CAP | NCP\_MSG\_TYPE\_RESP | 0x0000000c)

> Bluetooth LE l2cap receive command response ID

#### \#define NCP\_CMD\_BLE\_VENDOR\_POWER\_MODE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_CMD | 0x00000001)

> Bluetooth LE Vendor enable/disable power mode command ID

#### \#define NCP\_RSP\_BLE\_VENDOR\_POWER\_MODE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Bluetooth LE Vendor enable/disable power mode command response ID

#### \#define NCP\_CMD\_BLE\_VENDOR\_SET\_UART\_BR  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_CMD | 0x00000002)

> Bluetooth LE Vendor set uart baud rate command ID

#### \#define NCP\_RSP\_BLE\_VENDOR\_SET\_UART\_BR  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Bluetooth LE Vendor set uart baud rate command response ID

#### \#define NCP\_CMD\_BLE\_VENDOR\_SET\_DEVICE\_ADDR  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_CMD | 0x00000003)

> Bluetooth LE Vendor set uart device address command ID

#### \#define NCP\_RSP\_BLE\_VENDOR\_SET\_DEVICE\_ADDR  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Bluetooth LE Vendor set uart device address command response ID

#### \#define NCP\_CMD\_BLE\_VENDOR\_SET\_DEVICE\_NAME  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_CMD | 0x00000004)

> Bluetooth LE Vendor set device name command ID

#### \#define NCP\_RSP\_BLE\_VENDOR\_SET\_DEVICE\_NAME  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Bluetooth LE Vendor set device name command response ID

#### \#define NCP\_CMD\_BLE\_VENDOR\_CFG\_MULTI\_ADV  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_CMD | 0x00000005)

> Bluetooth LE Vendor config multi-advertising command ID

#### \#define NCP\_RSP\_BLE\_VENDOR\_CFG\_MULTI\_ADV  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_VENDOR | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Bluetooth LE Vendor config multi-advertising command response ID

#### \#define NCP\_EVENT\_IUT\_READY  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | CORE\_EV\_IUT\_READY)

> Bluetooth LE device ready event

#### \#define NCP\_EVENT\_ADV\_REPORT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_DEVICE\_FOUND)

> Bluetooth LE advertising report event

#### \#define NCP\_EVENT\_DEVICE\_CONNECTED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_DEVICE\_CONNECTED)

> Bluetooth LE connection complete event

#### \#define NCP\_EVENT\_DEVICE\_DISCONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_DEVICE\_DISCONNECTED)

> Bluetooth LE disconnection complete event

#### \#define NCP\_EVENT\_PASSKEY\_DISPLAY  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_PASSKEY\_DISPLAY)

> Bluetooth LE passkey display event

#### \#define NCP\_EVENT\_IDENTITY\_RESOLVED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_IDENTITY\_RESOLVED)

> Bluetooth LE remote identity address resolved event

#### \#define NCP\_EVENT\_CONN\_PARAM\_UPDATE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_CONN\_PARAM\_UPDATE)

> Bluetooth LE connection parameter update event

#### \#define NCP\_EVENT\_SEC\_LEVEL\_CHANGED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_SEC\_LEVEL\_CHANGED)

> Bluetooth LE security level changed event

#### \#define NCP\_EVENT\_PAIRING\_FAILED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_PAIRING\_FAILED)

> Bluetooth LE paring failed event

#### \#define NCP\_EVENT\_BOND\_LOST  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_BOND\_LOST)

> Bluetooth LE bond lost event

#### \#define NCP\_EVENT\_PHY\_UPDATED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_PHY\_UPDATED)

> Bluetooth LE phy update event

#### \#define NCP\_EVENT\_DATA\_LEN\_UPDATED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GAP\_EV\_DATA\_LEN\_UPDATED)

> Bluetooth LE data length update event

#### \#define NCP\_EVENT\_GATT\_NOTIFICATION  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GATT\_EV\_NOTIFICATION | 0x200)

> Bluetooth LE gatt notification received event

#### \#define NCP\_EVENT\_ATTR\_VALUE\_CHANGED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GATT\_EV\_ATTR\_VALUE\_CHANGED | 0x200)

> Bluetooth LE gatt attribute value changed event

#### \#define NCP\_EVENT\_GATT\_CCC\_CFG\_CHANGED  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GATT\_EV\_CCC\_CFG\_CHANGED | 0x200)

> Bluetooth LE gatt client characteristic configuration changed event

#### \#define NCP\_EVENT\_GATT\_SUBSCRIPTION  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | GATT\_EV\_SUBSCRIPTION | 0x200)

> Bluetooth LE gatt client subscription event

#### \#define NCP\_EVENT\_GATT\_DISC\_PRIM  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x19 | 0x200)

> Bluetooth LE gatt discover primary service event

#### \#define NCP\_EVENT\_GATT\_DISC\_CHRC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x20 | 0x200)

> Bluetooth LE gatt discover characteristic event

#### \#define NCP\_EVENT\_GATT\_DISC\_DESC  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x21 | 0x200)

> Bluetooth LE gatt discover descriptor event

#### \#define NCP\_EVENT\_L2CAP\_CONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | L2CAP\_EV\_CONNECT | 0x300)

> Bluetooth LE l2cap connect event

#### \#define NCP\_EVENT\_L2CAP\_DISCONNECT  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | L2CAP\_EV\_DISCONNECT | 0x300)

> Bluetooth LE l2cap disconnect event

#### \#define NCP\_EVENT\_L2CAP\_RECEIVE  (NCP\_CMD\_BLE | NCP\_CMD\_BLE\_EVENT | NCP\_MSG\_TYPE\_EVENT | L2CAP\_EV\_RECEIVE | 0x300)

> Bluetooth LE l2cap receive event

#### \#define NCP\_CMD\_GATT\_ADD\_SERVICE\_TLV  0x0001

> NCP Bluetooth LE TLV type NCP GATT add service tlv type

#### \#define NCP\_CMD\_GATT\_ADD\_CHRC\_TLV  0x0002

> NCP GATT add characteristic tlv type

#### \#define NCP\_CMD\_GATT\_ADD\_DESC\_TLV  0x0003

> NCP GATT add descriptor tlv type

#### \#define NCP\_CMD\_GATT\_START\_SVC\_TLV  0x0004

> NCP GATT start service tlv type

#### \#define NCP\_CMD\_GATT\_DISC\_PRIM\_TLV  0x0005

> NCP GATT discover primary service tlv type

#### \#define NCP\_CMD\_GATT\_DISC\_CHRC\_TLV  0x0006

> NCP GATT discover characteristic tlv type

#### \#define NCP\_CMD\_GATT\_DISC\_DESC\_TLV  0x0007

> NCP GATT discover descriptor tlv type

#### \#define BT\_UUID\_SIZE\_16  2

> Size in octets of a 16-bit UUID

#### \#define BT\_UUID\_SIZE\_128  16

> Size in octets of a 128-bit UUID

#### \#define BT\_HCI\_VD\_SET\_BD\_ADDRESS  BT\_OP(BT\_OGF\_VS, 0x0022)

> Bluetooth LE Vendor set board address hci opcode

#### \#define BT\_HCI\_VD\_LOW\_POWER\_MODE  BT\_OP(BT\_OGF\_VS, 0x0023)

> NCP Bluetooth LE low power mode opcode

#### \#define BT\_HCI\_VD\_MULTI\_ADV\_CMD  BT\_OP(BT\_OGF\_VS, 0x0154)

> NCP Bluetooth LE multi advertising opcode

#### \#define CORE\_RESET\_BOARD  0x06

> NCP Bluetooth LE Core reset board command ID

#### \#define GAP\_READ\_SUPPORTED\_COMMANDS  0x01

> NCP Bluetooth LE GAP read support command ID

#### \#define GAP\_READ\_CONTROLLER\_INDEX\_LIST  0x02

> NCP Bluetooth LE GAP read controller index list command ID

#### \#define GAP\_SETTINGS\_POWERED  0

> NCP Bluetooth LE GAP set power ID

#### \#define GAP\_SETTINGS\_CONNECTABLE  1

> NCP Bluetooth LE GAP connect ID

#### \#define GAP\_SETTINGS\_FAST\_CONNECTABLE  2

> NCP Bluetooth LE GAP fast connect ID

#### \#define GAP\_SETTINGS\_DISCOVERABLE  3

> NCP Bluetooth LE GAP discover ID

#### \#define GAP\_SETTINGS\_BONDABLE  4

> NCP Bluetooth LE GAP bond ID

#### \#define GAP\_SETTINGS\_LINK\_SEC\_3  5

> NCP Bluetooth LE GAP set security ID

#### \#define GAP\_SETTINGS\_SSP  6

> NCP Bluetooth LE GAP set SSP ID

#### \#define GAP\_SETTINGS\_BREDR  7

> NCP Bluetooth LE GAP set bredr ID

#### \#define GAP\_SETTINGS\_HS  8

> NCP Bluetooth LE GAP set hs ID

#### \#define GAP\_SETTINGS\_LE  9

> NCP Bluetooth LE GAP set le ID

#### \#define GAP\_SETTINGS\_ADVERTISING  10

> NCP Bluetooth LE GAP set advertising ID

#### \#define GAP\_SETTINGS\_SC  11

> NCP Bluetooth LE GAP set SC ID

#### \#define GAP\_SETTINGS\_DEBUG\_KEYS  12

> NCP Bluetooth LE GAP set debug key ID

#### \#define GAP\_SETTINGS\_PRIVACY  13

> NCP Bluetooth LE GAP set privacy ID

#### \#define GAP\_SETTINGS\_CONTROLLER\_CONFIG  14

> NCP Bluetooth LE GAP set controller config ID

#### \#define GAP\_SETTINGS\_STATIC\_ADDRESS  15

> NCP Bluetooth LE GAP set static address ID

#### \#define GAP\_NON\_DISCOVERABLE  0x00

> NCP Bluetooth LE GAP non discoverable ID

#### \#define GAP\_GENERAL\_DISCOVERABLE  0x01

> NCP Bluetooth LE GAP general discoverable ID

#### \#define GAP\_LIMITED\_DISCOVERABLE  0x02

> NCP Bluetooth LE GAP limit discoverable ID

#### \#define GAP\_SET\_DISCOVERABLE  0x08

> NCP Bluetooth LE GAP discoverable ID

#### \#define GAP\_SET\_BONDABLE  0x09

> NCP Bluetooth LE GAP set bondable ID

#### \#define GAP\_START\_ADVERTISING  0x0a

> NCP Bluetooth LE GAP set advertising ID

#### \#define GAP\_STOP\_ADVERTISING  0x0b

> NCP Bluetooth LE GAP stop advertising ID

#### \#define GAP\_DISCOVERY\_FLAG\_LE  0x01

> NCP Bluetooth LE GAP discovery le ID

#### \#define GAP\_DISCOVERY\_FLAG\_BREDR  0x02

> NCP Bluetooth LE GAP discovery bredr ID

#### \#define GAP\_DISCOVERY\_FLAG\_LIMITED  0x04

> NCP Bluetooth LE GAP discovery limited ID

#### \#define GAP\_DISCOVERY\_FLAG\_LE\_ACTIVE\_SCAN  0x08

> NCP Bluetooth LE GAP active scan ID

#### \#define GAP\_DISCOVERY\_FLAG\_LE\_OBSERVE  0x10

> NCP Bluetooth LE GAP observe ID

#### \#define GAP\_DISCOVERY\_FLAG\_OWN\_ID\_ADDR  0x20

> NCP Bluetooth LE GAP discovery own address ID

#### \#define GAP\_START\_DISCOVERY  0x0c

> NCP Bluetooth LE GAP start discovery ID

#### \#define GAP\_STOP\_DISCOVERY  0x0d

> NCP Bluetooth LE GAP stop discovery ID

#### \#define GAP\_CONNECT  0x0e

> NCP Bluetooth LE GAP connect ID

#### \#define GAP\_DISCONNECT  0x0f

> NCP Bluetooth LE GAP disconnect ID

#### \#define GAP\_IO\_CAP\_DISPLAY\_ONLY  0

> NCP Bluetooth LE GAP IO capability display only ID

#### \#define GAP\_IO\_CAP\_DISPLAY\_YESNO  1

> NCP Bluetooth LE GAP IO capability display option ID

#### \#define GAP\_IO\_CAP\_KEYBOARD\_ONLY  2

> NCP Bluetooth LE GAP IO capability keyboard only ID

#### \#define GAP\_IO\_CAP\_NO\_INPUT\_OUTPUT  3

> NCP Bluetooth LE GAP IO capability no input ouput ID

#### \#define GAP\_IO\_CAP\_KEYBOARD\_DISPLAY  4

> NCP Bluetooth LE GAP IO capability keyboard\&display ID

#### \#define GAP\_SET\_IO\_CAP  0x10

> NCP Bluetooth LE GAP IO capability ID

#### \#define GAP\_PAIR  0x11

> NCP Bluetooth LE GAP pair ID

#### \#define GAP\_UNPAIR  0x12

> NCP Bluetooth LE GAP unpair ID

#### \#define GAP\_PASSKEY\_ENTRY  0x13

> NCP Bluetooth LE GAP passkey entry ID

#### \#define GAP\_PASSKEY\_CONFIRM  0x14

> NCP Bluetooth LE GAP passkey confirm ID

#### \#define GAP\_START\_DIRECTED\_ADV\_HD  BIT(0)

> NCP Bluetooth LE start directed advertising hd

#### \#define GAP\_START\_DIRECTED\_ADV\_OWN\_ID  BIT(1)

> NCP Bluetooth LE start directed advertising own id

#### \#define GAP\_START\_DIRECTED\_ADV\_PEER\_RPA  BIT(2)

> NCP Bluetooth LE start directed advertising peer rpa

#### \#define GAP\_START\_DIRECTED\_ADV  0x15

> NCP Bluetooth LE start directed advertising ID

#### \#define GAP\_CONN\_PARAM\_UPDATE  0x16

> NCP Bluetooth LE GAP connection parameter update ID

#### \#define GAP\_PAIRING\_CONSENT  0x17

> NCP Bluetooth LE GAP pairing consent ID

#### \#define GAP\_OOB\_LEGACY\_SET\_DATA  0x18

> NCP Bluetooth LE oob legacy set data ID

#### \#define GAP\_OOB\_SC\_GET\_LOCAL\_DATA  0x19

> NCP Bluetooth LE oob security get local data ID

#### \#define GAP\_OOB\_SC\_SET\_REMOTE\_DATA  0x1a

> NCP Bluetooth LE oob security set remote data ID

#### \#define GAP\_SET\_MITM  0x1b

> NCP Bluetooth LE GAP set mitm ID

#### \#define GAP\_SET\_FILTER\_LIST  0x1c

> NCP Bluetooth LE GAP set filter list ID

#### \#define GAP\_SET\_SCAN\_PARAMETER  0x1d

> NCP Bluetooth LE GAP set scan parameter ID

#### \#define GAP\_SET\_ADV\_DATA  0x1e

> NCP Bluetooth LE GAP set advertising data ID

#### \#define GAP\_EV\_NEW\_SETTINGS  0x80

> NCP Bluetooth LE new setting event ID

#### \#define GAP\_DEVICE\_FOUND\_FLAG\_RSSI  0x01

> NCP Bluetooth LE device found flag

#### \#define GAP\_EV\_DEVICE\_FOUND  0x81

> NCP Bluetooth LE device found event ID

#### \#define GAP\_EV\_DEVICE\_CONNECTED  0x82

> NCP Bluetooth LE device connected event ID

#### \#define GAP\_EV\_DEVICE\_DISCONNECTED  0x83

> NCP Bluetooth LE device disconnected event ID

#### \#define GAP\_EV\_PASSKEY\_DISPLAY  0x84

> NCP Bluetooth LE passkey display event ID

#### \#define GAP\_EV\_PASSKEY\_ENTRY\_REQ  0x85

> NCP Bluetooth LE passkey entry request event ID

#### \#define GAP\_EV\_PASSKEY\_CONFIRM\_REQ  0x86

> NCP Bluetooth LE passkey confirm request event ID

#### \#define GAP\_EV\_IDENTITY\_RESOLVED  0x87

> NCP Bluetooth LE identity resolved event ID

#### \#define GAP\_EV\_CONN\_PARAM\_UPDATE  0x88

> NCP Bluetooth LE connection parameter update event ID

#### \#define GAP\_SEC\_LEVEL\_UNAUTH\_ENC  0x01

> NCP Bluetooth LE GAP security level

#### \#define GAP\_EV\_SEC\_LEVEL\_CHANGED  0x89

> NCP Bluetooth LE security level changed event ID

#### \#define GAP\_EV\_PAIRING\_CONSENT\_REQ  0x8a

> NCP Bluetooth LE pairing consent request event ID

#### \#define GAP\_EV\_BOND\_LOST  0x8b

> NCP Bluetooth LE bonding lost event ID

#### \#define GAP\_EV\_PAIRING\_FAILED  0x8c

> NCP Bluetooth LE pairing failed event ID

#### \#define GAP\_EV\_PHY\_UPDATED  0x91

> NCP Bluetooth LE phy update event ID

#### \#define GAP\_EV\_DATA\_LEN\_UPDATED  0x92

> NCP Bluetooth LE data length update event ID

#### \#define L2CAP\_READ\_SUPPORTED\_COMMANDS  0x01

> NCP Bluetooth LE l2cap read support command ID

#### \#define L2CAP\_CONNECT\_OPT\_ECFC  0x01

> NCP Bluetooth LE opt

#### \#define L2CAP\_CONNECT  0x02

> NCP Bluetooth LE l2cap connection ID

#### \#define L2CAP\_DISCONNECT  0x03

> NCP Bluetooth LE l2cap disconnect ID

#### \#define L2CAP\_SEND\_DATA  0x04

> NCP Bluetooth LE l2cap send data ID

#### \#define L2CAP\_TRANSPORT\_BREDR  0x00

> NCP Bluetooth LE l2cap transport type

#### \#define L2CAP\_CONNECTION\_RESPONSE\_SUCCESS  0x00

> NCP Bluetooth LE l2cap connection response type

#### \#define L2CAP\_LISTEN  0x05

> NCP Bluetooth LE l2cap listen ID

#### \#define L2CAP\_ACCEPT\_CONNECTION  0x06

> NCP Bluetooth LE l2cap accept connection ID

#### \#define L2CAP\_RECONFIGURE  0x07

> NCP Bluetooth LE l2cap reconfigure ID

#### \#define L2CAP\_CREDITS  0x08

> NCP Bluetooth LE l2cap credit ID

#### \#define L2CAP\_DISCONNECT\_EATT\_CHANS  0x09

> NCP Bluetooth LE l2cap disconnect eatt (Enhanced ATT Bearers support)
> channel ID

#### \#define L2CAP\_REGISTER\_PSM  0x0a

> NCP Bluetooth LE l2cap register PSM (Protocol/Service Multiplexer) ID

#### \#define L2CAP\_METRICS  0x0b

> NCP Bluetooth LE l2cap metrics ID

#### \#define L2CAP\_RECV  0x0c

> NCP Bluetooth LE l2cap receive ID

#### \#define L2CAP\_EV\_CONNECTION\_REQ  0x80

> NCP Bluetooth LE connection request event ID

#### \#define L2CAP\_EV\_CONNECTED  0x81

> NCP Bluetooth LE l2cap connect event ID

#### \#define L2CAP\_EV\_DISCONNECTED  0x82

> NCP Bluetooth LE l2cap disconnect event ID

#### \#define L2CAP\_EV\_DATA\_RECEIVED  0x83

> NCP Bluetooth LE l2cap data receive event ID

#### \#define L2CAP\_EV\_RECONFIGURED  0x84

> NCP Bluetooth LE l2cap reconfigure event ID

#### \#define SERVER\_MAX\_SERVICES  10

> gatt server context

#### \#define GATT\_READ\_SUPPORTED\_COMMANDS  0x01

> NCP Bluetooth LE gatt read support command ID

#### \#define GATT\_SERVICE\_PRIMARY  0x00

> NCP Bluetooth LE service type NCP Bluetooth LE primary service type

#### \#define GATT\_SERVICE\_SECONDARY  0x01

> NCP Bluetooth LE secondary service type

#### \#define GATT\_ADD\_SERVICE  0x02

> NCP Bluetooth LE gatt add service ID

#### \#define GATT\_ADD\_CHARACTERISTIC  0x03

> NCP Bluetooth LE gatt add characteristic ID

#### \#define GATT\_ADD\_DESCRIPTOR  0x04

> NCP Bluetooth LE gatt add descriptor ID

#### \#define GATT\_ADD\_INCLUDED\_SERVICE  0x05

> NCP Bluetooth LE gatt add include service ID

#### \#define GATT\_SET\_VALUE  0x06

> NCP Bluetooth LE gatt set value ID

#### \#define GATT\_START\_SERVER  0x07

> NCP Bluetooth LE gatt start server ID

#### \#define GATT\_RESET\_SERVER  0x08

> NCP Bluetooth LE gatt reset server ID

#### \#define GATT\_SET\_ENC\_KEY\_SIZE  0x09

> NCP Bluetooth LE gatt set encryption key size ID

#### \#define GATT\_EXCHANGE\_MTU  0x0a

> NCP Bluetooth LE exchange maximum transmission unit ID

#### \#define GATT\_DISC\_ALL\_PRIM  0x0b

> NCP Bluetooth LE discovery primary service ID

#### \#define GATT\_DISC\_PRIM\_UUID  0x0c

> NCP Bluetooth LE discovery primary UUID ID

#### \#define GATT\_FIND\_INCLUDED  0x0d

> NCP Bluetooth LE gatt find included ID

#### \#define GATT\_DISC\_ALL\_CHRC  0x0e

> NCP Bluetooth LE gatt discovery characteristic ID

#### \#define GATT\_DISC\_CHRC\_UUID  0x0f

> NCP Bluetooth LE gatt discovery characteristic UUID ID

#### \#define GATT\_DISC\_ALL\_DESC  0x10

> NCP Bluetooth LE gatt discovery descriptor ID

#### \#define GATT\_DISC\_DESC\_UUID  0x21

> NCP Bluetooth LE gatt discovery descriptor UUID ID

#### \#define GATT\_READ  0x11

> NCP Bluetooth LE gatt read ID

#### \#define GATT\_READ\_UUID  0x12

> NCP Bluetooth LE gatt read UUID ID

#### \#define GATT\_READ\_LONG  0x13

> NCP Bluetooth LE gatt read long ID

#### \#define GATT\_READ\_MULTIPLE  0x14

> NCP Bluetooth LE gatt read multiple ID

#### \#define GATT\_WRITE\_WITHOUT\_RSP  0x15

> NCP Bluetooth LE gatt write without response ID

#### \#define GATT\_SIGNED\_WRITE\_WITHOUT\_RSP  0x16

> NCP Bluetooth LE gatt signed write without response ID

#### \#define GATT\_WRITE  0x17

> NCP Bluetooth LE gatt write ID

#### \#define GATT\_WRITE\_LONG  0x18

> NCP Bluetooth LE gatt write long ID

#### \#define GATT\_RELIABLE\_WRITE  0x19

> NCP Bluetooth LE gatt write reliable ID

#### \#define GATT\_CFG\_NOTIFY  0x1a

> NCP Bluetooth LE gatt config

#### \#define GATT\_GET\_ATTRIBUTES  0x1c

> NCP Bluetooth LE gatt get attribute ID

#### \#define GATT\_GET\_ATTRIBUTE\_VALUE  0x1d

> NCP Bluetooth LE gatt get attribute value ID

#### \#define GATT\_CHANGE\_DB  0x1e

> NCP Bluetooth LE gatt change DB ID

#### \#define GATT\_EATT\_CONNECT  0x1f

> NCP Bluetooth LE gatt eatt connect ID

#### \#define GATT\_READ\_MULTIPLE\_VAR  0x20

> NCP Bluetooth LE gatt read multiple variable ID

#### \#define GATT\_NOTIFY\_MULTIPLE  0x21

> NCP Bluetooth LE gatt notify multiple ID

#### \#define GATT\_NCP\_BLE\_ADD\_SERVICE  0x22

> NCP Bluetooth LE add service ID

#### \#define GATT\_EV\_NOTIFICATION  0x80

> NCP Bluetooth LE gatt notification event

#### \#define GATT\_EV\_ATTR\_VALUE\_CHANGED  0x81

> NCP Bluetooth LE gatt attribute value change event

#### \#define GATT\_EV\_CCC\_CFG\_CHANGED  0x82

> NCP Bluetooth LE gatt client characteristic configuration declaration
> event ID

#### \#define GATT\_EV\_SUBSCRIPTION  0x85

> NCP Bluetooth LE gatt subscription event ID

#### \#define L2CAP\_EV\_CONNECT  0x81

> NCP Bluetooth LE l2cap connect event ID

#### \#define L2CAP\_EV\_DISCONNECT  0x82

> NCP Bluetooth LE l2cap disconnect event ID

#### \#define L2CAP\_EV\_RECEIVE  0x83

> NCP Bluetooth LE l2cap receive event ID

#### \#define MAX\_ATTRIBUTE\_VALUE\_LEN  256

> The maximum length of the attribute to notify

#### 

## ncp\_cmd\_wifi.h File Reference

NCP Wi-Fi command/response definitions.

### Macro Documentation

#### \#define NCP\_CMD\_WLAN\_STA  0x00000000

> NCP Wi-Fi subclass types subclass type for Wi-Fi station commands,
> such as scan, connect, version, set mac and so on.

#### \#define NCP\_CMD\_WLAN\_BASIC  0x00100000

> subclass type for basic command such as reset, reset UAP provisioning
> and start UAP provisioning.

#### \#define NCP\_CMD\_WLAN\_REGULATORY  0x00200000

> subclass type for regulatory commands, such as set RF TX, RX antenna
> mode and RF band.

#### \#define NCP\_CMD\_WLAN\_POWERMGMT  0x00300000

> subclass type for power management commands, such as enable/disable
> deep sleep power save mode.

#### \#define NCP\_CMD\_WLAN\_DEBUG  0x00400000

> subclass type for debug commands.

#### \#define NCP\_CMD\_WLAN\_OTHER  0x00500000

> subclass type for TWT (Target wake-up time), 802.11ax and region code
> commands.

#### \#define NCP\_CMD\_WLAN\_MEMORY  0x00600000

> subclass type for memory commands.

#### \#define NCP\_CMD\_WLAN\_NETWORK  0x00700000

> subclass type for network commands, such as add, start, stop and
> remove network command.

#### \#define NCP\_CMD\_WLAN\_SOCKET  0x00900000

> subclass type for socket command, such as socket open, connect, close
> and send.

#### \#define NCP\_CMD\_WLAN\_UAP  0x00a00000

> subclass type for UAP commands

#### \#define NCP\_CMD\_WLAN\_HTTP  0x00b00000

> subclass type for http commands

#### \#define NCP\_CMD\_WLAN\_ASYNC\_EVENT  0x00f00000

> subclass type for async events.

#### \#define NCP\_WLAN\_PASSWORD\_MAX\_LENGTH  255

> Maximum length of password string

#### \#define IEEEtypes\_MAC\_ADDRESS\_SIZE  6

> MAC address length.

#### \#define MLAN\_MAX\_DRIVER\_VER\_STR\_LEN  16

> Maximum length of Wi-Fi driver version string.

#### \#define MLAN\_IPV6\_ADDR\_LEN  4

> System's IPv6 address.

#### \#define IPV6\_MAX\_ADDR\_TYPE\_STR\_LEN  16

> Maximum length of IPV6 address type string.

#### \#define IPV6\_MAX\_ADDR\_STATE\_STR\_LEN  32

> Maximum address state string of IPv6 address.

#### \#define NCP\_WLAN\_KNOWN\_NETWORKS  5

> Maximum size of the network list

#### \#define NCP\_MDNS\_LABEL\_MAX\_LEN  63

> MDNS label maximum length

#### \#define NCP\_CMD\_WLAN\_STA\_SCAN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-scan \*/

> Wi-Fi STA scan command ID

#### \#define NCP\_RSP\_WLAN\_STA\_SCAN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi STA scan command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_CONNECT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-connect \*/

> Wi-Fi STA connect command ID

#### \#define NCP\_RSP\_WLAN\_STA\_CONNECT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP |0x00000002)

> Wi-Fi STA connect command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_DISCONNECT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-disconnect \*/

> Wi-Fi STA disconnect command ID

#### \#define NCP\_RSP\_WLAN\_STA\_DISCONNECT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi STA disconnect command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_VERSION  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-version \*/

> Wi-Fi STA version command ID

#### \#define NCP\_RSP\_WLAN\_STA\_VERSION  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi STA version command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_SET\_MAC  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-set-mac \*/

> Wi-Fi STA set MAC address command ID

#### \#define NCP\_RSP\_WLAN\_STA\_SET\_MAC  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi STA set MAC address command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_GET\_MAC  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-get-mac \*/

> Wi-Fi STA get MAC address command ID

#### \#define NCP\_RSP\_WLAN\_STA\_GET\_MAC  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi STA get MAC address command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_CONNECT\_STAT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000007) /\* wlan-stat \*/

> Wi-Fi STA connect state command ID

#### \#define NCP\_RSP\_WLAN\_STA\_CONNECT\_STAT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> Wi-Fi STA connect state command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_ROAMING  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-roaming \*/

> Wi-Fi STA roaming command ID

#### \#define NCP\_RSP\_WLAN\_STA\_ROAMING  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi STA roaming command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000009) /\* wlan-set-antenna / wlan-get-antenna\*/

> Wi-Fi STA set/get antenna command ID

#### \#define NCP\_RSP\_WLAN\_STA\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000009)

> Wi-Fi STA set/get antenna command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_SIGNAL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000012) /\* wlan-get-signal \*/

> Wi-Fi STA signal command ID

#### \#define NCP\_RSP\_WLAN\_STA\_SIGNAL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000012)

> Wi-Fi STA signal command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_CSI  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000020) /\* wlan-csi \*/

> Wi-Fi STA csi command ID

#### \#define NCP\_RSP\_WLAN\_STA\_CSI  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000020)

> Wi-Fi STA csi command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_11K\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000021) /\* wlan-11k-enable \*/

> Wi-Fi STA enable 11K command ID

#### \#define NCP\_RSP\_WLAN\_STA\_11K\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000021)

> Wi-Fi STA enable 11K command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_NEIGHBOR\_REQ  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000022) /\* wlan-11k-neighbor-req \*/

> Wi-Fi STA neighbor request command ID

#### \#define NCP\_RSP\_WLAN\_STA\_NEIGHBOR\_REQ  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000022)

> Wi-Fi STA neighbor request command response ID

#### \#define NCP\_CMD\_WLAN\_MBO\_ENABLE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000023) /\*wlan-mbo-enable\*/

> Wi-Fi STA MBO enable command ID

#### \#define NCP\_RSP\_WLAN\_MBO\_ENABLE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000023)

> Wi-Fi STA MBO enable command response ID

#### \#define NCP\_CMD\_WLAN\_MBO\_NONPREFER\_CH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000024) /\*wlan-mbo-nonprefer-ch\*/

> Wi-Fi STA MBO non prefer channel command ID

#### \#define NCP\_RSP\_WLAN\_MBO\_NONPREFER\_CH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000024)

> Wi-Fi STA MBO non prefer channel command response ID

#### \#define NCP\_CMD\_WLAN\_MBO\_SET\_CELL\_CAPA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000025) /\*wlan-mbo-set-cell-capa\*/

> Wi-Fi STA MBO set cell capacity command ID

#### \#define NCP\_RSP\_WLAN\_MBO\_SET\_CELL\_CAPA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000025)

> Wi-Fi STA MBO set cell capacity command response ID

#### \#define NCP\_CMD\_WLAN\_MBO\_SET\_OCE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000026) /\*wlan-mbo-set-oce\*/

> Wi-Fi STA MBO set OCE command ID

#### \#define NCP\_RSP\_WLAN\_MBO\_SET\_OCE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000026)

> Wi-Fi STA MBO set OCE command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_WPS\_PBC  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000031) /\* wlan-start-wps-pbc \*/

> Wi-Fi STA start WPS PBC (push button configuration) command ID

#### \#define NCP\_RSP\_WLAN\_STA\_WPS\_PBC  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000031)

> Wi-Fi STA start WPS PBC command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_GEN\_WPS\_PIN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000032) /\* wlan-generate-wps-pin \*/

> Wi-Fi STA generate WPS pin code command ID

#### \#define NCP\_RSP\_WLAN\_STA\_GEN\_WPS\_PIN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000032)

> Wi-Fi STA generate WPS pin code command response ID

#### \#define NCP\_CMD\_WLAN\_STA\_WPS\_PIN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD |0x00000033) /\* wlan-start-wps-pin \*/

> Wi-Fi STA start WPS pin command ID

#### \#define NCP\_RSP\_WLAN\_STA\_WPS\_PIN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000033)

> Wi-Fi STA start WPS pin command response ID

#### \#define NCP\_CMD\_WLAN\_GET\_CURRENT\_NETWORK  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000034)

> Wi-Fi STA get current network command ID

#### \#define NCP\_RSP\_WLAN\_GET\_CURRENT\_NETWORK  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000034)

> Wi-Fi STA get current network command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORKS\_REMOVE\_ALL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_CMD | 0x00000035)

> Wi-Fi STA remove all network command ID

#### \#define NCP\_RSP\_WLAN\_NETWORKS\_REMOVE\_ALL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_STA | NCP\_MSG\_TYPE\_RESP | 0x00000035)

> Wi-Fi STA remove all network command response ID

#### \#define NCP\_CMD\_WLAN\_BASIC\_WLAN\_RESET  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-reset \*/

> WLAN Basic command/response Wi-Fi reset command ID

#### \#define NCP\_RSP\_WLAN\_BASIC\_WLAN\_RESET  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi reset command response ID

#### \#define NCP\_CMD\_WLAN\_BASIC\_WLAN\_UAP\_PROV\_START  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-uap-prov-start \*/

> Wi-Fi start UAP provisioning command ID

#### \#define NCP\_RSP\_WLAN\_BASIC\_WLAN\_UAP\_PROV\_START  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi start UAP provisioning command response ID

#### \#define NCP\_CMD\_WLAN\_BASIC\_WLAN\_UAP\_PROV\_RESET  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-uap-prov-reset \*/

> Wi-Fi reset UAP provisioning command ID

#### \#define NCP\_RSP\_WLAN\_BASIC\_WLAN\_UAP\_PROV\_RESET  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi reset UAP provisioning command response ID

#### \#define NCP\_CMD\_WLAN\_BASIC\_WLAN\_UAP\_PROV\_SET\_UAPCFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-uap-prov-set-uapcfg \*/

> Wi-Fi UAP provisioning config command ID

#### \#define NCP\_RSP\_WLAN\_BASIC\_WLAN\_UAP\_PROV\_SET\_UAPCFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_BASIC | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi UAP provisioning config command response ID

#### \#define NCP\_CMD\_WLAN\_HTTP\_CON  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-http-connect \*/

> WLAN Http command and response Wi-Fi http connect command ID

#### \#define NCP\_RSP\_WLAN\_HTTP\_CON  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi http connect command response ID

#### \#define NCP\_CMD\_WLAN\_HTTP\_DISCON  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-http-disconnect \*/

> Wi-Fi http disconnect command ID

#### \#define NCP\_RSP\_WLAN\_HTTP\_DISCON  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi http disconnect command response ID

#### \#define NCP\_CMD\_WLAN\_HTTP\_REQ  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-http-req \*/

> Wi-Fi http request command ID

#### \#define NCP\_RSP\_WLAN\_HTTP\_REQ  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi http request command response ID

#### \#define NCP\_CMD\_WLAN\_HTTP\_RECV  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-http-recv \*/

> Wi-Fi http receive command ID

#### \#define NCP\_RSP\_WLAN\_HTTP\_RECV  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi http request command response ID

#### \#define NCP\_CMD\_WLAN\_HTTP\_SETH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-http-seth \*/

> Wi-Fi http set header command ID

#### \#define NCP\_RSP\_WLAN\_HTTP\_SETH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi http set header command response ID

#### \#define NCP\_CMD\_WLAN\_HTTP\_UNSETH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-http-unseth \*/

> Wi-Fi http unset header command ID

#### \#define NCP\_RSP\_WLAN\_HTTP\_UNSETH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi http unset header command response ID

#### \#define NCP\_CMD\_WLAN\_WEBSOCKET\_UPG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000007) /\* wlan-websocket-upg \*/

> Wi-Fi web socket update command ID

#### \#define NCP\_RSP\_WLAN\_WEBSOCKET\_UPG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> Wi-Fi web socket update command response ID

#### \#define NCP\_CMD\_WLAN\_WEBSOCKET\_SEND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-websocket-send \*/

> Wi-Fi web socket send command ID

#### \#define NCP\_RSP\_WLAN\_WEBSOCKET\_SEND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi web socket send command response ID

#### \#define NCP\_CMD\_WLAN\_WEBSOCKET\_RECV  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_CMD | 0x00000009) /\* wlan-websocket-recv \*/

> Wi-Fi web socket receive command ID

#### \#define NCP\_RSP\_WLAN\_WEBSOCKET\_RECV  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_HTTP | NCP\_MSG\_TYPE\_RESP | 0x00000009)

> Wi-Fi web socket receive command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_OPEN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-socket-open \*/

> WLAN Socket command Wi-Fi socket open command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_OPEN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi socket open command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_CON  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-socket-connect \*/

> Wi-Fi socket connect command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_CON  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi socket connect command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_RECV  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-socket-receive \*/

> Wi-Fi socket receive command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_RECV  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi socket receive command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_SEND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-socket-send \*/

> Wi-Fi socket send command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_SEND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi socket send command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_SENDTO  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-socket-sendto \*/

> Wi-Fi socket sendto command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_SENDTO  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi socket sendto command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_BIND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-socket-bind \*/

> Wi-Fi socket bind command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_BIND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi socket bind command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_LISTEN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000007) /\* wlan-socket-listen \*/

> Wi-Fi socket listen command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_LISTEN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> Wi-Fi socket listen command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_ACCEPT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-socket-accept \*/

> Wi-Fi socket accept command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_ACCEPT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi socket accept command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_CLOSE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x00000009) /\* wlan-socket-close \*/

> Wi-Fi socket close command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_CLOSE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x00000009)

> Wi-Fi socket close command response ID

#### \#define NCP\_CMD\_WLAN\_SOCKET\_RECVFROM  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_CMD | 0x0000000a) /\* wlan-socket-recvfrom \*/

> Wi-Fi socket receive from command ID

#### \#define NCP\_RSP\_WLAN\_SOCKET\_RECVFROM  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_SOCKET | NCP\_MSG\_TYPE\_RESP | 0x0000000a)

> Wi-Fi socket receive from command response ID

#### \#define NCP\_CMD\_WLAN\_DEBUG\_REGISTER\_ACCESS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_DEBUG | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-reg-access \*/

> Wi-Fi Debug command/response Wi-Fi register access command ID

#### \#define NCP\_RSP\_WLAN\_DEBUG\_REGISTER\_ACCESS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_DEBUG | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi register access command response ID

#### \#define NCP\_CMD\_WLAN\_MEMORY\_HEAP\_SIZE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_MEMORY | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-mem-stat \*/

> WLAN Memory command Wi-Fi memory state command ID

#### \#define NCP\_RSP\_WLAN\_MEMORY\_HEAP\_SIZE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_MEMORY | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi memory state command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_INFO  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-info \*/

> WLAN Network command/response Wi-Fi network information command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_INFO  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi network information command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_MONITOR  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-monitor \*/

> Wi-Fi network monitor command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_MONITOR  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi network monitor command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_ADD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-add \*/

> Wi-Fi add network command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_ADD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi add network command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_START  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-start-network \*/

> Wi-Fi start network command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_START  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi start network command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_STOP  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-stop-network \*/

> Wi-Fi stop network command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_STOP  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi stop network command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_GET\_UAP\_STA\_LIST  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-get-uap-sta-list \*/

> Wi-Fi get number of STAs connected to UAP command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_GET\_UAP\_STA\_LIST  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi get number of STAs connected to UAP command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_MDNS\_QUERY  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000007) /\* wlan-mdns-query \*/

> Wi-Fi MDNS query command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_MDNS\_QUERY  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> Wi-Fi MDNS query command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_LIST  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-list \*/

> Wi-Fi get added network list command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_LIST  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi get added network list command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_REMOVE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x00000009) /\* wlan-remove \*/

> Wi-Fi remove already added network command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_REMOVE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x00000009)

> Wi-Fi remove already added network command response ID

#### \#define NCP\_CMD\_WLAN\_NETWORK\_ADDRESS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_CMD | 0x0000000A) /\* wlan-address \*/

> Wi-Fi get STA/UAP network IP address command ID

#### \#define NCP\_RSP\_WLAN\_NETWORK\_ADDRESS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_NETWORK | NCP\_MSG\_TYPE\_RESP | 0x0000000A)

> Wi-Fi get STA/UAP network IP address command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_MEF  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-multi-mef \*/

> Wi-Fi configure multiple MEF (memory efficient filtering) command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_MEF  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi configure multiple MEF command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_DEEP\_SLEEP\_PS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-deep-sleep-ps \*/

> Wi-Fi enable/disable deep sleep power save mode command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_DEEP\_SLEEP\_PS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi enable/disable deep sleep power save mode command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_IEEE\_PS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-ieee-ps \*/

> Wi-Fi enable/disable IEEE power save mode command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_IEEE\_PS  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi enable/disable IEEE power save mode command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_UAPSD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-uapsd-enable \*/

> Wi-Fi enable/disable WMM (Wi-Fi multimedia) power save mode command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_UAPSD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi enable/disable WMM power save mode command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_QOSINFO  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-uapsd-qosinfo \*/

> Wi-Fi set/get WMM QOS (quality of service) information command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_QOSINFO  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi set/get WMM QOS information command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_SLEEP\_PERIOD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-uapsd-sleep-period \*/

> Wi-Fi set/get sleep period command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_SLEEP\_PERIOD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi set/get sleep period command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_WOWLAN\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-wowlan-cfg \*/

> Wi-Fi set wakeup conditions command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_WOWLAN\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi set wakeup conditions command response ID

#### \#define NCP\_CMD\_WLAN\_POWERMGMT\_SUSPEND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_CMD | 0x0000000a) /\* wlan-suspend \*/

> Wi-Fi device enter power save mode command ID

#### \#define NCP\_RSP\_WLAN\_POWERMGMT\_SUSPEND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_POWERMGMT | NCP\_MSG\_TYPE\_RESP | 0x0000000a)

> Wi-Fi device enter power save mode command response ID

#### \#define NCP\_CMD\_WLAN\_UAP\_MAX\_CLIENT\_CNT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_UAP | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-set-max-clients-count \*/

> WLAN UAP command Wi-Fi set maximum client count command ID

#### \#define NCP\_RSP\_WLAN\_UAP\_MAX\_CLIENT\_CNT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_UAP | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi set maximum client count command response ID

#### \#define NCP\_CMD\_11AX\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-set-11axcfg \*/

> WLAN other command Wi-Fi configure 802.11ax features command ID

#### \#define NCP\_RSP\_11AX\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi configure 802.11ax features command response ID

#### \#define NCP\_CMD\_BTWT\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-set-11axcfg \*/

> Wi-Fi configure broadcast TWT command ID

#### \#define NCP\_RSP\_BTWT\_CFG  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi configure broadcast TWT command response ID

#### \#define NCP\_CMD\_TWT\_SETUP  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-set-btwt-cfg \*/

> Wi-Fi setup TWT command ID

#### \#define NCP\_RSP\_TWT\_SETUP  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi setup TWT command response ID

#### \#define NCP\_CMD\_TWT\_TEARDOWN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-twt-teardown \*/

> Wi-Fi disable TWT command ID

#### \#define NCP\_RSP\_TWT\_TEARDOWN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi disable TWT command response ID

#### \#define NCP\_CMD\_TWT\_GET\_REPORT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-get-twt-report \*/

> Wi-Fi get TWT information report command ID

#### \#define NCP\_RSP\_TWT\_GET\_REPORT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi get TWT information report command response ID

#### \#define NCP\_CMD\_11D\_ENABLE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-set-11d-enable \*/

> ID of Wi-Fi enable/disable STA/UAP 802.11d features command

#### \#define NCP\_RSP\_11D\_ENABLE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi enable/disable STA/UAP 802.11d features command response ID

#### \#define NCP\_CMD\_REGION\_CODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000007) /\* wlan-region-code \*/

> ID of Wi-Fi set/get region code command

#### \#define NCP\_RSP\_REGION\_CODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> ID of Wi-Fi set/get region code command response

#### \#define NCP\_CMD\_DATE\_TIME  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-set/get-time \*/

> Wi-Fi set/get time date command ID

#### \#define NCP\_RSP\_DATE\_TIME  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi set/get time date command response ID

#### \#define NCP\_CMD\_GET\_TEMPERATUE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x00000009) /\* wlan-get-temp \*/

> Wi-Fi get chip temperature command ID

#### \#define NCP\_RSP\_GET\_TEMPERATUE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x00000009)

> Wi-Fi get chip temperature command response ID

#### \#define NCP\_CMD\_INVALID\_CMD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_CMD | 0x0000000a)

> Wi-Fi invalid command ID

#### \#define NCP\_RSP\_INVALID\_CMD  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_OTHER | NCP\_MSG\_TYPE\_RESP | 0x0000000a)

> Wi-Fi invalid command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_ED\_MAC\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000001) /\* wlan-set-ed-mac-mode \*/

> Wi-Fi enable/disable EU (encrypt unit) adaptivity command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_ED\_MAC\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000001)

> Wi-Fi enable/disable EU adaptivity command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_TEST\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000002) /\* wlan-set-rf-test-mode \*/

> Wi-Fi set RF test mode command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_TEST\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000002)

> Wi-Fi set RF test mode command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_TX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000003) /\* wlan-set-rf-tx-antenna \*/

> Wi-Fi set RF transmit antenna command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_TX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000003)

> Wi-Fi set RF transmit antenna command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_RF\_TX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000004) /\* wlan-get-rf-tx-antenna \*/

> Wi-Fi get RF transmit antenna command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_RF\_TX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000004)

> Wi-Fi get RF transmit antenna command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_RX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000005) /\* wlan-set-rf-rx-antenna \*/

> Wi-Fi set RF receive antenna command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_RX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000005)

> Wi-Fi set RF receive antenna command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_RF\_RX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000006) /\* wlan-get-rf-rx-antenna \*/

> Wi-Fi get RF receive antenna command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_RF\_RX\_ANTENNA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000006)

> Wi-Fi get RF receive antenna command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_BAND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000007) /\* wlan-set-rf-band \*/

> Wi-Fi set RF band command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_BAND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000007)

> Wi-Fi set RF band command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_RF\_BAND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000008) /\* wlan-get-rf-band \*/

> Wi-Fi get RF band command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_RF\_BAND  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000008)

> Wi-Fi get RF band command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_BANDWIDTH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000009) /\* wlan-set-rf-bandwidth \*/

> Wi-Fi set RF bandwidth command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_BANDWIDTH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000009)

> Wi-Fi set RF bandwidth command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_RF\_BANDWIDTH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x0000000a) /\* wlan-get-rf-bandwidth \*/

> Wi-Fi get RF bandwidth command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_RF\_BANDWIDTH  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x0000000a)

> Wi-Fi get RF bandwidth command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_CHANNEL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x0000000b) /\* wlan-set-rf-channel \*/

> Wi-Fi set RF channel number command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_CHANNEL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x0000000b)

> Wi-Fi set RF channel number command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_RF\_CHANNEL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x0000000c) /\* wlan-get-rf-channel \*/

> Wi-Fi get RF channel number command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_RF\_CHANNEL  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x0000000c)

> Wi-Fi get RF channel number command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_RADIO\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x0000000d) /\* wlan-set-rf-radio-mode \*/

> Wi-Fi set RF radio mode command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_RADIO\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x0000000d)

> Wi-Fi set RF radio mode command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_RF\_RADIO\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x0000000e) /\* wlan-get-rf-radio-mode \*/

> Wi-Fi get RF radio mode command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_RF\_RADIO\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x0000000e)

> Wi-Fi get RF radio mode command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_TX\_POWER  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x0000000f) /\* wlan-set-rf-tx-power \*/

> Wi-Fi set RF transmit power command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_TX\_POWER  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x0000000f)

> Wi-Fi set RF transmit power command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_TX\_CONT\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000010) /\* wlan-set-rf-tx-cont-mode \*/

> Wi-Fi set RF transmit continuous configuration command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_TX\_CONT\_MODE  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000010)

> Wi-Fi set RF transmit continuous configuration command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_SET\_RF\_TX\_FRAME  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000011) /\* wlan-set-rf-tx-frame \*/

> Wi-Fi set RF transmit frame command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_SET\_RF\_TX\_FRAME  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000011)

> Wi-Fi set RF transmit frame command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_GET\_AND\_RESET\_RF\_PER  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000012) /\* wlan-get-and-reset-rf-per \*/

> Wi-Fi get and reset RF per command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_GET\_AND\_RESET\_RF\_PER  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000012)

> Wi-Fi get and reset RF per command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_CCMP\_128  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000013) /\* wlan-eu-crypto-ccmp-128 \*/

> Wi-Fi verify Algorithm AES-CCMP-128 encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_CCMP\_128  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000013)

> Wi-Fi verify Algorithm AES-CCMP-128 encryption decryption command
> response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_GCMP\_128  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000014) /\* wlan-eu-crypto-gcmp-128 \*/

> Wi-Fi verify Algorithm AES-GCMP-128 encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_GCMP\_128  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000014)

> Wi-Fi verify Algorithm AES-GCMP-128 encryption decryption command
> response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_CCMP\_256  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000015) /\* wlan-eu-crypto-ccmp-256 \*/

> Wi-Fi verify Algorithm AES-CCMP-256 encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_CCMP\_256  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000015)

> Wi-Fi verify Algorithm AES-CCMP-256 encryption decryption command
> response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_GCMP\_256  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000016) /\* wlan-eu-crypto-gcmp-256 \*/

> Wi-Fi verify Algorithm AES-GCMP-256 encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_GCMP\_256  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000016)

> Wi-Fi verify Algorithm AES-GCMP-256 encryption decryption command
> response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_RC4  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000017) /\* wlan-eu-crypto-rc4 \*/

> Wi-Fi verify Algorithm RC4 encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_RC4  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000017)

> Wi-Fi verify Algorithm RC4 encryption decryption command response ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_AES\_WRAP  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000018) /\* wlan-eu-crypto-aes-wrap \*/

> Wi-Fi verify Algorithm AES-WRAP encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_AES\_WRAP  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000018)

> Wi-Fi verify Algorithm AES-WRAP encryption decryption command response
> ID

#### \#define NCP\_CMD\_WLAN\_REGULATORY\_EU\_CRYPTO\_AES\_ECB  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_CMD | 0x00000019) /\* wlan-eu-crypto-aes-ecb \*/

> Wi-Fi verify Algorithm AES-ECB encryption decryption command ID

#### \#define NCP\_RSP\_WLAN\_REGULATORY\_EU\_CRYPTO\_AES\_ECB  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_REGULATORY | NCP\_MSG\_TYPE\_RESP | 0x00000019)

> Wi-Fi verify Algorithm AES-ECB encryption decryption command response
> ID

#### \#define NCP\_EVENT\_MDNS\_QUERY\_RESULT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_ASYNC\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x00000003) /\* mdns-query-result \*/

> WLAN events Wi-Fi MDNS query result event ID

#### \#define NCP\_EVENT\_MDNS\_RESOLVE\_DOMAIN  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_ASYNC\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x00000004) /\* mdns-resolve-domain-name \*/

> Wi-Fi MDNS resolve domain event ID

#### \#define NCP\_EVENT\_CSI\_DATA  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_ASYNC\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x00000005) /\* csi data \*/

> Wi-Fi MDNS CSI (channel state information) data event ID

#### \#define NCP\_EVENT\_WLAN\_STA\_CONNECT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_ASYNC\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x00000006) /\* wlan sta connect \*/

> Wi-Fi station connect AP event ID

#### \#define NCP\_EVENT\_WLAN\_STA\_DISCONNECT  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_ASYNC\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x00000007) /\* wlan sta disconnect \*/

> Wi-Fi station disconnect AP event ID

#### \#define NCP\_EVENT\_WLAN\_STOP\_NETWORK  (NCP\_CMD\_WLAN | NCP\_CMD\_WLAN\_ASYNC\_EVENT | NCP\_MSG\_TYPE\_EVENT | 0x00000008) /\* wlan stop network \*/

> Wi-Fi UAP stop network event ID

#### \#define NCP\_CMD\_NETWORK\_SSID\_TLV  0x0001

> NCP WLAN TLV NCP network ssid tlv type

#### \#define NCP\_CMD\_NETWORK\_BSSID\_TLV  0x0002

> NCP network BSSID tlv type

#### \#define NCP\_CMD\_NETWORK\_CHANNEL\_TLV  0x0003

> NCP network channel tlv type

#### \#define NCP\_CMD\_NETWORK\_IP\_TLV  0x0004

> NCP network IP tlv type

#### \#define NCP\_CMD\_NETWORK\_SECURITY\_TLV  0x0005

> NCP network security tlv type

#### \#define NCP\_CMD\_NETWORK\_ROLE\_TLV  0x0006

> NCP network role tlv type

#### \#define NCP\_CMD\_NETWORK\_DTIM\_TLV  0x0007

> NCP network DTIM tlv type

#### \#define NCP\_CMD\_NETWORK\_CAPA\_TLV  0x0008

> NCP network capacity tlv type

#### \#define NCP\_CMD\_NETWORK\_ACSBAND\_TLV  0x0009

> NCP auto change band tlv type

#### \#define NCP\_CMD\_NETWORK\_PMF\_TLV  0x000A

> NCP management frame protection tlv type

#### \#define NCP\_CMD\_NETWORK\_PWE\_TLV  0x000B

> NCP network PWE (password element) tlv type

#### \#define NCP\_CMD\_NETWORK\_TR\_TLV  0x000C

> NCP transition\_Disable tlv type

#### \#define NCP\_CMD\_NETWORK\_EAP\_TLV  0x000D

> NCP network EAP tlv type

#### \#define NCP\_CMD\_WLAN\_HE\_CAP\_TLV  0x00FF

> NCP network high efficiency tlv type

#### \#define NCP\_CMD\_NETWORK\_MDNS\_RESULT\_PTR  0x0011

> NCP MDNS result TLV NCP PTR (pointer records) resource record tlv type

#### \#define NCP\_CMD\_NETWORK\_MDNS\_RESULT\_SRV  0x0012

> NCP SRV (service records) resource record tlv type

#### \#define NCP\_CMD\_NETWORK\_MDNS\_RESULT\_TXT  0x0013

> NCP TXT (text) resource record tlv type

#### \#define NCP\_CMD\_NETWORK\_MDNS\_RESULT\_IP\_ADDR  0x0014

> NCP IPv4 and IPv6 resource record tlv type

#### \#define WLAN\_SET\_MAX\_CLIENT\_CNT\_SUCCESS  0

> Set UAP max client count status

#### \#define IP\_ADDR\_LEN  16

> NCP Wi-Fi socket connect

#### \#define SETH\_NAME\_LENGTH  64

> NCP Wi-Fi Http Seth

#### \#define HTTP\_URI\_LEN  512

> NCP Wi-Fi http request

#### 

## ncp\_host\_command\_ble.h File Reference

NCP Bluetooth LE host command interfaces.

### Function Documentation

#### int ble\_set\_adv\_data\_command (int *argc*, char \*\* *argv*)

> This API is used to set Bluetooth LE advertising data

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 2</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-set-adv-data'</p>
<p>argv[1]: Advertising data to set (Required)</p>
<p>Advertising data should follow structure:</p>
<p>struct bt_data {</p>
<p>U8 len;</p>
<p>U8 type;</p>
<p>const U8 *data;</p>
<p>}</p>
<p>Example: 0e094e43505f444542554731323334</p>
<p>Detail as:</p>
<p>len : 0e(14 bytes, contain type and data)</p>
<p>type: 09(BT_DATA_NAME_COMPLETE)</p>
<p>data: 4e43505f444542554731323334(NCP_DEBUG1234)</p>
<p>TYPE Reference: Core Specification Supplement, Part A, Data Types Specification</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_start\_adv\_command (int *argc*, char \*\* *argv*)

> This API is used to start Bluetooth LE advertising

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 1</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-start-adv'</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_stop\_adv\_command (int *argc*, char \*\* *argv*)

> This API is used to stop Bluetooth LE advertising

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 1</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-stop-adv'</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_set\_scan\_param\_command (int *argc*, char \*\* *argv*)

> This API is used to set Bluetooth LE scan parameter

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 4</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-set-scan-param'</p>
<p>argv[1]: filter option (Required)</p>
<p>0 – BT_LE_SCAN_OPT_NONE (Convenience value when no options are specified)</p>
<p>1 – BT_LE_SCAN_OPT_FILTER_DUPLICATE (Filter duplicates)</p>
<p>2 – BT_LE_SCAN_OPT_FILTER_ACCEPT_LIST (Filter using filter accept list)</p>
<p>argv[2]: scan interval (Required)</p>
<p>Range from 4 to 16384 (Decimal value)</p>
<p>argv[3]: scan window (Required)</p>
<p>Range: 4 to 16384 (Decimal value, window value shall be less than or equal to interval value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_start\_scan\_command (int *argc*, char \*\* *argv*)

> This API is used to start Bluetooth LE scanning

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 2</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-start-scan'</p>
<p>argv[1] – scan type (Required)</p>
<p>0 – active scan</p>
<p>1 – passive scan</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_stop\_scan\_command (int *argc*, char \*\* *argv*)

> This API is used to stop Bluetooth LE scanning

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 1</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-stop-scan'</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_connect\_command (int *argc*, char \*\* *argv*)

> This API is used to connect the advertising device

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 3</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-connect'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_disconnect\_command (int *argc*, char \*\* *argv*)

> This API is used to disconnect the connected device

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 3</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-disconnect'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_connect\_parameter\_update\_command (int *argc*, char \*\* *argv*)

> This API is used to update the device connection parameter

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 7</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-conn-param-update'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[3]: connection maximum interval (Required)</p>
<p>Maximum value for the connection interval. This shall be greater than or equal to minimum connection interval. (Decimal value)</p>
<p>argv[4]: connection minimum interval (Required)</p>
<p>Minimum value for the connection interval. This shall be less than or equal to maximum connection interval. (Decimal value)</p>
<p>argv[5]: connection latency</p>
<p>Maximum Peripheral latency for the connection in number of subrated connection events. (Decimal value)</p>
<p>argv[6]: connection timeout</p>
<p>Supervision timeout for the Bluetooth LE link. (Decimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_set\_phy\_command (int *argc*, char \*\* *argv*)

> This API is used to set the Bluetooth PHY

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 5</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-set-phy'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[3]: Bluetooth TX PHY</p>
<p>1 – 1M PHY</p>
<p>2 – 2M PHY</p>
<p>4 – Coded PHY</p>
<p>argv[4]: Bluetooth RX PHY</p>
<p>1 – 1M PHY</p>
<p>2 – 2M PHY</p>
<p>4 – Coded PHY</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_set\_data\_len\_command (int *argc*, char \*\* *argv*)

> This API is used to update data packet length

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 4 or 5</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-set-data-len'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[3]: Maximum data length to transmit (Required)</p>
<p>Range: 27 to 251 (Decimal value)</p>
<p>argv[4]: Maximum TX transmit time (Optional)</p>
<p>Range: 328 to 17040 (Decimal value, this parameter can be left unset when sending commands)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_set\_filter\_list\_command (int *argc*, char \*\* *argv*)

> This API is used to configure the filter list. After the filter list
> is set, use the ble-scan-param to scan the parameter. Select the
> filter option 2. Send the scan command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be at least 4</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-set-filter-list'</p>
<p>argv[1]: The number of device address need to filter (Required)</p>
<p>Range: 1 to 255 (Decimal value)</p>
<p>argv[2]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[3]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>......</p>
<p>argv[n-1]: address type (Optional)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[n]: device address (Optional)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_start\_encryption\_command (int *argc*, char \*\* *argv*)

> This API is used to initiate security connection. If peer is already
> paired, IUT (Implementation under test) is expected to enable security
> (encryption) with peer. If peer is not paired, the IUT start the
> pairing process.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 3</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-start-encryption'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_read\_characteristic\_command (int *argc*, char \*\* *argv*)

> This API is used to read profile characteristic value by handle. This
> API must be called after the device is connected

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 4</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-read-characteristic'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[3]: attribute handle (Required)</p>
<p>Range from 0x0000 ~ 0xffff (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_register\_service\_command (int *argc*, char \*\* *argv*)

> This API is used to register the service that runs the profile on the
> NCP device

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be at least 3</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-register-service'</p>
<p>argv[1]: The number of service to register (Required)</p>
<p>Range from 1 to 5</p>
<p>argv[2]: service id (Required)</p>
<p>1 – Peripheral HTS (Health Thermometer)</p>
<p>2 – Peripheral HRS (Health Rate)</p>
<p>3 – Peripheral BAS (Battery Service)</p>
<p>4 – Central HTS</p>
<p>5 – Central HRS</p>
<p>.....</p>
<p>argv[n]: service id (Optional)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_set\_power\_mode\_command (int *argc*, char \*\* *argv*)

> This API is used to set the device power mode

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 2</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-set-power-mode'</p>
<p>argv[1]: device power mode</p>
<p>0 – enable controller auto sleep</p>
<p>1 – disable controller auto sleep</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_host\_service\_add\_command (int *argc*, char \*\* *argv*)

> This API is used to set the service parameters. One service may have
> several characteristic

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be at least 11</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-host-svc-add'</p>
<p>argv[1]: 'prim' (Required)</p>
<p>argv[2]: primary service uuid (Required)</p>
<p>XXXX (Hexadecimal value)</p>
<p>argv[3]: 'chrc' (Required)</p>
<p>argv[4]: characteristic uuid (Required)</p>
<p>XXXX (Hexadecimal value)</p>
<p>argv[5]: characteristic properties (Required)</p>
<p>XX (Hexadecimal value)</p>
<p>BIT 1: Broadcast</p>
<p>BIT 2: Read</p>
<p>BIT 3: Write Without Response</p>
<p>BIT 4: Write</p>
<p>BIT 5: Notify</p>
<p>BIT 6: Indicate</p>
<p>BIT 7: Authenticated Signed Writes</p>
<p>BIT 8: Extended Properties</p>
<p>argv[6]: characteristic permission (Required)</p>
<p>XX (Hexadecimal value)</p>
<p>0: None</p>
<p>BIT 0: Read</p>
<p>BIT 1: Write</p>
<p>BIT 2: Read with Encryption</p>
<p>BIT 3: Write with Encryption</p>
<p>BIT 4: Read with Authentication</p>
<p>BIT 5: Write with Authentication</p>
<p>BIT 6: Prepare Write</p>
<p>BIT 7: Read with LE Secure Connection encryption</p>
<p>BIT 8: Write with LE Secure Connection encryption</p>
<p>argv[7]: 'ccc' (Required)</p>
<p>argv[8]: client characteristic configuration uuid (Required)</p>
<p>XXXX (Hexadecimal value)</p>
<p>argv[9]: client characteristic configuration permissions (Required)</p>
<p>XX (Hexadecimal value)</p>
<p>0: None</p>
<p>BIT 0: Read</p>
<p>BIT 1: Write</p>
<p>BIT 2: Read with Encryption</p>
<p>BIT 3: Write with Encryption</p>
<p>BIT 4: Read with Authentication</p>
<p>BIT 5: Write with Authentication</p>
<p>BIT 6: Prepare Write</p>
<p>BIT 7: Read with LE Secure Connection encryption</p>
<p>BIT 8: Write with LE Secure Connection encryption</p>
<p>......</p>
<p>argv[n]: 'start' (Required)</p>
<p>Example: Add hts service</p>
<p>ble-host-svc-add prim 1809 chrc 2a1c 20 00 ccc 2902 03 start (same as the command 'ble-start-service hts' )</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_start\_service\_command (int *argc*, char \*\* *argv*)

> This API is used to start service that runs the profile on the NCP
> host

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 2</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-start-service'</p>
<p>argv[1]: profile name (Required)</p>
<p>'hts' – Peripheral health thermometer</p>
<p>'htc' – Client health thermometer</p>
<p>'hrs' – Peripheral health rate</p>
<p>'hrc' – Client health rate</p>
<p>'bas' – Peripheral battery service</p></td>
</tr>
</tbody>
</table>

##### Note

> To use the command, enable the following macros:
> 
> CONFIG\_NCP\_BLE
> 
> CONFIG\_NCP\_BLE\_PROFILE\_MODE
> 
> CONFIG\_NCP\_HTS
> 
> CONFIG\_NCP\_HRS
> 
> CONFIG\_NCP\_HTC
> 
> CONFIG\_NCP\_HRC

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_cfg\_subscribe\_command (int *argc*, char \*\* *argv*)

> This API is used to subscribe the ccc handle

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 6</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-cfg-subscribe'</p>
<p>argv[1]: property type (Required)</p>
<p>'indicate' – If set, changes to Characteristic Value are indicated</p>
<p>'notify' – If set, changes to Characteristic Value are notified</p>
<p>argv[2]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[3]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[4]: enable subscription option (Required)</p>
<p>0 – disable subscription</p>
<p>1 – enable subscription</p>
<p>argv[5]: client characteristic configuration handle (Required)</p>
<p>specific one needs to be referred to the print ccc handle value of Discover the Descriptor Event (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_l2cap\_connection\_command (int *argc*, char \*\* *argv*)

> This API is used to connect the l2cap PSM (Protocol service
> multiplexer).
> 
> The l2cap PSM need to be registered by remote device, then issue
> ble-l2cap-connect command to connect the remote device

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 4</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-l2cap-connect'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[3]: Protocol service multiplexer value (Required)</p>
<p>Range: 128 to 255 (Decimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_l2cap\_disconnect\_command (int *argc*, char \*\* *argv*)

> This API is used to disconnect the l2cap channel

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 3</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-l2cap-disconnect'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_l2cap\_send\_command (int *argc*, char \*\* *argv*)

> This API is used to send the data with the l2cap PSM

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 4</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-l2cap-send'</p>
<p>argv[1]: address type (Required)</p>
<p>'public' – public address</p>
<p>'random' – random address</p>
<p>argv[2]: device address (Required)</p>
<p>device address format: XX:XX:XX:XX:XX:XX (Hexadecimal value)</p>
<p>argv[3]: times</p>
<p>the number of times a loop is sent (Decimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### int ble\_l2cap\_register\_command (int *argc*, char \*\* *argv*)

> This API is used to register the l2cap PSM

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv.</p>
<p>argc should be 2</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: 'ble-l2cap-register'</p>
<p>argv[1]: Protocol service multiplexer value (Required)</p>
<p>Range: 128 to 255 (Decimal value)</p></td>
</tr>
</tbody>
</table>

##### Returns

> NCP\_STATUS\_SUCCESS if success
> 
> NCP\_STATUS\_ERROR if failure

#### MCU\_NCPCmd\_DS\_BLE\_COMMAND\* ncp\_host\_get\_cmd\_buffer\_ble (void )

> Get NCP Bluetooth LE command buffer

##### Returns

> A pointer to MCU\_NCPCmd\_DS\_BLE\_COMMAND

#### 

## ncp\_host\_command\_wifi.h File Reference

NCP host command interfaces.

### Function Documentation

#### MCU\_NCPCmd\_DS\_COMMAND\* ncp\_host\_get\_cmd\_buffer\_wifi (void )

> Get NCP host TLV command buffer.

##### Returns

> pointer to the host TLV command buffer.

#### void ping\_wait\_event (osa\_event\_flags\_t *flagsToWait*)

> This API is used for ping task to wait for user input ping command
> from console.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flagsToWait</em></td>
<td>Event flags that to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void ping\_set\_event (osa\_event\_flags\_t *flagsToWait*)

> This API is used to send ping command response to ping\_sock\_task.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flagsToWait</em></td>
<td>Event flags that to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void iperf\_tx\_wait\_event (osa\_event\_flags\_t *flagsToWait*)

> This API is used to wait for transmit event.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flagsToWait</em></td>
<td>Event flags that to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void iperf\_tx\_set\_event (osa\_event\_flags\_t *flagsToWait*)

> This API is used to set transmit event flags that to wait.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flagsToWait</em></td>
<td>Event flags that to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void iperf\_rx\_wait\_event (osa\_event\_flags\_t *flagsToWait*)

> This API is used to wait for receive event IPERF\_RX\_START.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flagsToWait</em></td>
<td>Event flags that to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### void iperf\_rx\_set\_event (osa\_event\_flags\_t *flagsToWait*)

> This API is used to set receive event flags.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>flagsToWait</em></td>
<td>Event flags that to wait</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### int wlan\_uap\_prov\_start\_command (int *argc*, char \*\* *argv*)

> This API is used to start the UAP provisioning.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-uap-prov-start</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_process\_wlan\_uap\_prov\_start\_result\_response (uint8\_t \* *res*)

> This API is used to process start the UAP provisioning response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_uap\_prov\_reset\_command (int *argc*, char \*\* *argv*)

> This API is used to reset the UAP provisioning.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-uap-prov-reset</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_uap\_prov\_set\_uapcfg (int *argc*, char \*\* *argv*)

> This API is used to configure the UAP provisioning.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2, 3 or 4.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-uap-prov-set-uapcfg</p>
<p>argv[1]: ssid</p>
<p>argv[2]: security type</p>
<p>argv[3]: password string</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_process\_wlan\_uap\_prov\_reset\_result\_response (uint8\_t \* *res*)

> This API is used to process reset the UAP provisioning response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_get\_signal\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RSSI information.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-signal</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_process\_rssi\_response (uint8\_t \* *res*)

> This API is used to process the RSSI information response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RSSI.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_set\_antenna\_cfg\_command (int *argc*, char \*\* *argv*)

> This API is used to set the mode of TX/RX antenna.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2 or 3.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector</p>
<p>argv[0]: wlan-set-antenna-cfg</p>
<p>argv[1]: string of antenna mode (Required)</p>
<p>0: TX/RX antenna 1</p>
<p>1: TX/RX antenna 2</p>
<p>15: TX/RX antenna diversity.</p>
<p>argv[2]: string of evaluate_time (Optional)</p>
<p>if ant mode = 15, SAD (software antenna diversity) evaluate time interval.</p>
<p>default value is 6s(6000).</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_get\_antenna\_cfg\_command (int *argc*, char \*\* *argv*)

> This API is used to get the mode of TX/RX antenna.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-antenna-cfg</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_antenna\_cfg\_response (uint8\_t \* *res*)

> This API is used to process set/get antenna configuration response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_ANTENNA_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_set\_max\_clients\_count\_command (int *argc*, char \*\* *argv*)

> This API is used to set maximum number of the stations that can be
> allowed to connect to the UAP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-max-clients-count</p>
<p>argv[1]: string value of STA count, maximum supported STA count is 8.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if successful.
> 
> \-WM\_FAIL if unsuccessful.

##### Note

> Set operation in not allowed in WLAN\_UAP\_STARTED state.

#### int wlan\_process\_client\_count\_response (uint8\_t \* *res*)

> This API is used to process set the maximum number of stations
> response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_CLIENT_CNT.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_ccmp128\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm AES-CCMP-128 encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-ccmp-128</p>
<p>argv[1]: string value of decrypt or encypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_ccmp128\_response (uint8\_t \* *res*)

> This API is used to process the algorithm AES-CCMP-128 encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_gcmp128\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm AES-GCMP-128 encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-gcmp-128</p>
<p>argv[1]: string value of decrypt or encypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_gcmp128\_response (uint8\_t \* *res*)

> This API is used to process the algorithm AES-GCMP-128 encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_gcmp256\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm AES-GCMP-256 encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-gcmp-256</p>
<p>argv[1]: string value of decrypt or encypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_gcmp256\_response (uint8\_t \* *res*)

> This API is used to process the algorithm AES-GCMP-256 encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_ccmp256\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm AES-CCMP-256 encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-ccmp-256</p>
<p>argv[1]: string value of decrypt or encrypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_ccmp256\_response (uint8\_t \* *res*)

> This API is used to process the algorithm AES-CCMP-256 encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_rc4\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm RC4 encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-rc4</p>
<p>argv[1]: string value of decrypt or encrypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_rc4\_response (uint8\_t \* *res*)

> This API is used to process the algorithm RC4 encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_aes\_wrap\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm AES-WRAP encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-aes-wrap</p>
<p>argv[1]: string value of decrypt or encrypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_aes\_wrap\_response (uint8\_t \* *res*)

> This API is used to process the algorithm AES-WRAP encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_eu\_crypto\_aes\_ecb\_command (int *argc*, char \*\* *argv*)

> This API is used to verify the algorithm AES-ECB encryption and
> decryption.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-eu-crypto-aes-ecb</p>
<p>argv[1]: string value of decrypt or encrypt option.</p>
<p>0: decrypt</p>
<p>1: encrypt</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_eu\_crypto\_aes\_ecb\_response (uint8\_t \* *res*)

> This API is used to process the algorithm AES-ECB encryption and
> decryption response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_ed\_mac\_mode\_set\_command (int *argc*, char \*\* *argv*)

> This API is used to configure the ED (energy detect) MAC mode for
> station in Wi-Fi firmware.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>If enable CONFIG_NCP_5GHz_SUPPORT:</p>
<p>argc should be 5.</p>
<p>If disable CONFIG_NCP_5GHz_SUPPORT:</p>
<p>argc should be 3.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-ed-mac-mode</p>
<p>argv[1]: string of ed_ctrl_2g</p>
<p>0: disable EU adaptivity for 2.4GHz band.</p>
<p>1: enable EU adaptivity for 2.4GHz band.</p>
<p>argv[2]: string of ed_offset_2g</p>
<p>0: default dnergy detect threshold.</p>
<p>ed_threshold = ed_base - ed_offset_2g</p>
<p>e.g., if ed_base default is -62dBm, ed_offset_2g is 0x8, then ed_threshold is -70dBm.</p>
<p>#if CONFIG_NCP_5GHz_SUPPORT</p>
<p>argv[3]: string of ed_ctrl_5g</p>
<p>0: disable EU adaptivity for 5GHz band.</p>
<p>1: enable EU adaptivity for 5GHz band.</p>
<p>argv[4]: string of ed_offset_5g</p>
<p>0: default energy detect threshold.</p>
<p>ed_threshold = ed_base - ed_offset_5g</p>
<p>e.g., if ed_base default is -62dBm, ed_offset_5g is 0x8, then ed_threshold is -70dBm.</p>
<p>#endif</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_ed\_mac\_mode\_get\_command (int *argc*, char \*\* *argv*)

> This API is used to get the current ED MAC mode configuration for
> station.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-ed-mac-mode</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_ed\_mac\_response (uint8\_t \* *res*)

> This API is used to process the response for the set/get ED (energy
> detect) MAC mode command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_ED_MAC.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_set\_rf\_test\_mode\_command (int *argc*, char \*\* *argv*)

> This API is used to enable the RF test mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-test-mode</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> If you test with RF test mode, don't use wlan-reset 2, it is not
> supported.

#### int wlan\_process\_set\_rf\_test\_mode\_response (uint8\_t \* *res*)

> This API is used to process Wi-Fi set the RF test mode response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_set\_rf\_tx\_antenna\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF TX antenna mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-tx-antenna</p>
<p>argv[1]: antenna</p>
<p>1 – Main</p>
<p>2 – Aux</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_tx\_antenna\_response (uint8\_t \* *res*)

> This API is used to process the Wi-Fi set RF TX antenna response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_get\_rf\_tx\_antenna\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RF TX antenna mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-rf-tx-antenna</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Please set RF TX antenna before get it.

#### int wlan\_process\_get\_rf\_tx\_antenna\_response (uint8\_t \* *res*)

> This API is used to process the get RF TX antenna response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_TX_ANTENNA.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_rf\_rx\_antenna\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF RX antenna mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-rx-antenna</p>
<p>argv[1]: antenna</p>
<p>1 – Main</p>
<p>2 – Aux</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_rx\_antenna\_response (uint8\_t \* *res*)

> This API is used to process the response of the Wi-Fi set RF RX
> antenna mode command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_get\_rf\_rx\_antenna\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RF RX antenna mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-rf-rx-antenna</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Please set RF RX antenna before get it.

#### int wlan\_process\_get\_rf\_rx\_antenna\_response (uint8\_t \* *res*)

> This API is used to process the response of the get RF RX antenna mode
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_RX_ANTENNA.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_rf\_band\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF band.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-band</p>
<p>argv[1]: band</p>
<p>0 – 2.4G</p>
<p>1 – 5G</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_band\_response (uint8\_t \* *res*)

> This API is used to process the response of the configure RF band
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_get\_rf\_band\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RF band.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-rf-band</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Please set the RF band before get it.

#### int wlan\_process\_get\_rf\_band\_response (uint8\_t \* *res*)

> This API is used to process the response of the getting RF band
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_BAND.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_rf\_bandwidth\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF bandwidth.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-bandwidth</p>
<p>argv[1]: bandwidth</p>
<p>0 – 20MHz</p>
<p>1 – 40MHz</p>
<p>4 – 80MHz</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_bandwidth\_response (uint8\_t \* *res*)

> This API is used to process the response of the set RF bandwidth
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_get\_rf\_bandwidth\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RF bandwidth.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-rf-bandwidth</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Please set the RF bandwidth before get it.

#### int wlan\_process\_get\_rf\_bandwidth\_response (uint8\_t \* *res*)

> This API is used to process the response of the get RF bandwidth
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_BANDWIDTH.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_rf\_channel\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF channel.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-channel</p>
<p>argv[1]: channel, 2.4G channel numbers or 5G channel numbers</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_channel\_response (uint8\_t \* *res*)

> This API is used to process the response of the set RF channel
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_get\_rf\_channel\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RF channel.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-rf-channel</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Please set the RF channel before get it.

#### int wlan\_process\_get\_rf\_channel\_response (uint8\_t \* *res*)

> This API is used to process the response of the get RF channel
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_CHANNEL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_rf\_radio\_mode\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF radio mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-radio-mode</p>
<p>argv[1]: radio_mode</p>
<p>0 – set the radio in power down mode</p>
<p>3 – set the radio in 5GHz band, 1X1 mode(path A)</p>
<p>11 – set the radio in 2.4GHz band, 1X1 mode(path A)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_radio\_mode\_response (uint8\_t \* *res*)

> This API is used to process the response of setting RF radio mode
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_get\_rf\_radio\_mode\_command (int *argc*, char \*\* *argv*)

> This API is used to get the RF radio mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-rf-radio-mode</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Please set RF radio mode before get it.

#### int wlan\_process\_get\_rf\_radio\_mode\_response (uint8\_t \* *res*)

> This API is used to process the response of getting RF radio mode
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_RADIO_MODE.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_ncp\_set\_rf\_tx\_power\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF TX power.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 4.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-tx-power</p>
<p>argv[1]: power</p>
<p>0 to 24 (dBm)</p>
<p>argv[2]: modulation</p>
<p>0 – CCK</p>
<p>1 – OFDM</p>
<p>2 – MCS</p>
<p>argv[3]: path ID</p>
<p>0 – PathA</p>
<p>1 – PathB</p>
<p>2 – PathA+B</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_tx\_power\_response (uint8\_t \* *res*)

> This API is used to process the response for the setting RF TX power
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_ncp\_set\_rf\_tx\_cont\_mode\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF TX continuous mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2 or 6.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-tx-cont-mode</p>
<p>argv[1]: enable/disable RF TX cont mode (Required)</p>
<p>0 – disable RF TX cont mode</p>
<p>1 – enable RF TX cont mode</p>
<p>argv[2]: continuous wave Mode (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable continuous Wave Mode</p>
<p>1 – enable continuous Wave Mode</p>
<p>argv[3]: payload Pattern (Optional)</p>
<p>Required when argv[1] is 1</p>
<p>0 to 0xFFFFFFFF (Enter hexadecimal value)</p>
<p>argv[4]: CS mode (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>Applicable only when continuous wave is disabled.</p>
<p>0 – disable CS mode</p>
<p>1 – enable CS mode</p>
<p>argv[5]: Active SubChannel (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – low</p>
<p>1 – upper</p>
<p>3 – both</p>
<p>argv[6]: TX Data Rate (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>Rate index corresponding to legacy/HT/VHT rates.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_tx\_cont\_mode\_response (uint8\_t \* *res*)

> This API is used to process the response message for the setting RF TX
> continuous mode command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_ncp\_set\_rf\_tx\_frame\_command (int *argc*, char \*\* *argv*)

> This API is used to set the RF TX frame.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 4.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-set-rf-tx-frame</p>
<p>argv[1]: enable/disable RF TX frame (Required)</p>
<p>0 – disable RF TX frame</p>
<p>1 – enable RF TX frame</p>
<p>argv[2]: TX data rate (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>Rate index corresponding to legacy/HT/VHT rates).</p>
<p>argv[3]: Payload Pattern (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 to 0xFFFFFFFF (Enter hexadecimal value)</p>
<p>argv[4]: Payload Length (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>1 to 0x400 (Enter hexadecimal value)</p>
<p>argv[5]: Adjust burst SIFS3 gap (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[6]: Burst SIFS in us (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 to 255 (us)</p>
<p>argv[7]: Short preamble (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[8]: active subchannel (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – low</p>
<p>1 – upper</p>
<p>3 – both</p>
<p>argv[9]: short GI (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[10]: adv coding (Optional).</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[11]: Beamforming (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[12]: GreenField Mode (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[13]: STBC (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[14]: BSSID (Optional)</p>
<p>Required when argv[1] is 1.</p>
<p>xx:xx:xx:xx:xx:xx</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_tx\_frame\_response (uint8\_t \* *res*)

> This API is used to process set RF TX frame response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_ncp\_set\_rf\_get\_and\_reset\_rf\_per\_command (int *argc*, char \*\* *argv*)

> This API is used to get and reset RF per.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-get-and-reset-rf-per</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_rf\_get\_and\_reset\_rf\_per\_response (uint8\_t \* *res*)

> This API is used to process the response for the get and reset RF per
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_RF_PER.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_register\_access\_command (int *argc*, char \*\* *argv*)

> This API is used to reads/writes adapter registers value.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv. argc should be 3 or 4.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-reg-access</p>
<p>argv[1]: type (Required)</p>
<p>1: MAC</p>
<p>2: BBP</p>
<p>3: RF</p>
<p>4: CAU</p>
<p>argv[2]: offset (Required)</p>
<p>offset value of register.</p>
<p>argv[3]: value (Optional)</p>
<p>Set register value.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_register\_access\_response (uint8\_t \* *res*)

> This API is used to process the response message for the reads/writes
> adapter registers value command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_REGISTER_ACCESS.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_memory\_state\_command (int *argc*, char \*\* *argv*)

> This API is used to get the OS memory allocate and free info.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-mem-stat</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_process\_memory\_state\_response (uint8\_t \* *res*)

> This API is used to process the response message for the get OS memory
> allocate and free info command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_MEM_STAT.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_scan\_command (int *argc*, char \*\* *argv*)

> This API is used to scan for Wi-Fi networks.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-scan</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_scan\_response (uint8\_t \* *res*)

> This API is used to process the response message for the scan Wi-Fi
> network command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_SCAN_NETWORK_INFO.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_connect\_command (int *argc*, char \*\* *argv*)

> This API is used to connect to a Wi-Fi network (access point).

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-connect</p>
<p>argv[1]: string value of name (Required)</p>
<p>A string representing the name of the network to connect to.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_con\_response (uint8\_t \* *res*)

> This API is used to process the response for the connect command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_WLAN_CONN.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_disconnect\_command (int *argc*, char \*\* *argv*)

> This API is used to disconnect from the current Wi-Fi network (access
> point).

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-disconnect</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_discon\_response (uint8\_t \* *res*)

> This API is used to process the response for the disconnect command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_version\_command (int *argc*, char \*\* *argv*)

> This API is used to get the Wi-Fi driver and firmware extended
> version.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-version</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_version\_response (uint8\_t \* *res*)

> This API is used to process the response for the wlan-version command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_FW_VERSION.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_stat\_command (int *argc*, char \*\* *argv*)

> This API is used to retrieve the connection state of the station and
> UAP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-stat</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_stat\_response (uint8\_t \* *res*)

> This API is used to process the response of the Wi-Fi connection state
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_CONNECT_STAT.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_reset\_command (int *argc*, char \*\* *argv*)

> This API is used to reset the Wi-Fi driver.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-reset</p>
<p>argv[1]: action (Required)</p>
<p>0: disable Wi-Fi</p>
<p>1: enable Wi-Fi</p>
<p>2: reset Wi-Fi</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.
> 
> This API is used to control Wi-Fi system enable, disable and reset.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: should be "wlan-reset"</p>
<p>argv[1]: string value of Wi-Fi reset type (Required)</p>
<p>0 – disable Wi-Fi</p>
<p>1 – enable Wi-Fi</p>
<p>2 – reset Wi-Fi</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_reset\_response (uint8\_t \* *res*)

> This API is used to process the response for the Wi-Fi reset command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_mac\_address\_command (int *argc*, char \*\* *argv*)

> This API is used to set Wi-Fi MAC Address in Wi-Fi firmware.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-set-mac</p>
<p>argv[1]: string value of MAC (Required)</p>
<p>The MAC address format like "xx:xx:xx:xx:xx:xx".</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_set\_mac\_address (uint8\_t \* *res*)

> This API is used to process the response for the set Wi-Fi MAC address
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_get\_mac\_address\_command (int *argc*, char \*\* *argv*)

> This API is used to get Wi-Fi MAC Address in Wi-Fi firmware.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-get-mac</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_get\_mac\_address (uint8\_t \* *res*)

> This API is used to process the response for the get MAC address
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_GET_MAC_ADDRESS.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_info\_command (int *argc*, char \*\* *argv*)

> This API is used to Get the configured Wi-Fi network information.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-info</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_info (uint8\_t \* *res*)

> This API is used to process the response for the get Wi-Fi network
> information command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_NETWORK_INFO.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_mdns\_query\_command (int *argc*, char \*\* *argv*)

> This API is used to get the list of discovered service on the local
> network.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 3.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-mdns-query</p>
<p>argv[1]: string value of service types (Required)</p>
<p>The type of service to be discovered.</p>
<p>The service types can be found at http://www.dns-sd.org/ServiceTypes.html.</p>
<p>argv[2]: string value of protocol (Required)</p>
<p>e.g. TCP or UDP</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_mdns\_query\_response (uint8\_t \* *res*)

> This API is used to process the response for the MDNS query command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_mdns\_query\_result\_event (uint8\_t \* *res*)

> This API is used to process the MDNS query event.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Event body refer to NCP_EVT_MDNS_RESULT.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int ncp\_ping\_command (int *argc*, char \*\* *argv*)

> This API is used to send an ICMP echo request, receive its response
> and print its statistics and result.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 2 to 5.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: ping</p>
<p>argv[1]: value of -s &lt;packet_size&gt; (Optional)</p>
<p>argv[2]: value of -c &lt;packet_count&gt; (Optional)</p>
<p>argv[3]: value of -W &lt;timeout in sec&gt; (Optional)</p>
<p>argv[4]: value of &lt;ipv4 address&gt; (Required)</p>
<p>The ipv4 address of target device.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_add\_command (int *argc*, char \*\* *argv*)

> This API is used to add a network profile to the list of known
> networks.
> 
> The network's 'name' field is unique and between
> WLAN\_NETWORK\_NAME\_MIN\_LENGTH and WLAN\_NETWORK\_NAME\_MAX\_LENGTH
> characters.

##### Note

> The network must specify at least an SSID or BSSID.
> 
> This API is used to add profiles for station or UAP interfaces.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 3 to 14.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-add</p>
<p>argv[1]: string value of profile name (Required)</p>
<p>The name of network profile.</p>
<p>argv[2]: string value of ssid (Optional)</p>
<p>The network SSID, represented as a C string of up to 32 characters in length.</p>
<p>argv[3]: string value of ip address (Optional)</p>
<p>The ip address format like "ip:&lt;ip_addr&gt;,&lt;gateway_ip&gt;,&lt;netmask&gt;".</p>
<p>The network IP address configuration specified by struct</p>
<p>NCP_WLAN_IPV4_CONFIG that should be associated with this interface.</p>
<p>If this profile is used in the UAP mode, this field is mandatory.</p>
<p>If this profile is used in the station mode, this field is mandatory</p>
<p>if using static IP, and is optional if using DHCP.</p>
<p>argv[4]: string value of bssid (Optional)</p>
<p>The network BSSID, represented as a 6-byte array.</p>
<p>If this profile is used in the UAP mode, this field is ignored.</p>
<p>If this profile is used in the station mode, this field is used to</p>
<p>identify the network. Set all 6 bytes to 0 to use any BSSID, in which</p>
<p>case only the SSID is used to find the network.</p>
<p>argv[5]: string value of role (Required)</p>
<p>The network Wi-Fi mode enum wlan_bss_role.</p>
<p>Set this to specify what type of Wi-Fi network mode to use.</p>
<p>This can either be WLAN_BSS_ROLE_STA for use in the station mode,</p>
<p>or it can be WLAN_BSS_ROLE_UAP for use in the UAP mode.</p>
<p>argv[6]: string value of security (Optional)</p>
<p>The network security configuration specified for the network.</p>
<p>argv[7]: channel (Optional)</p>
<p>The channel for this network.</p>
<p>If this profile is used in UAP mode, this field specifies the channel to</p>
<p>start the UAP interface on. Set this to 0 for auto channel selection.</p>
<p>If this profile is used in the station mode, this constrains the channel on</p>
<p>which the network to connect should be present. Set this to 0 to allow the</p>
<p>network to be found on any channel.</p>
<p>argv[8]: capa (Optional)</p>
<p>Wi-Fi capabilities of UAP network 802.11n, 802.11ac or/and 802.11ax.</p>
<p>argv[9]: mfpc (Optional)</p>
<p>Management frame protection capable (MFPC)</p>
<p>argv[10]: mfpr (Optional)</p>
<p>Management frame protection required (MFPR)</p>
<p>argv[11]: dtim (Optional)</p>
<p>DTIM period of associated BSS</p>
<p>argv[12]: aid (Optional)</p>
<p>Client anonymous identity</p>
<p>argv[13]: string value of key_passwd (Optional)</p>
<p>Client Key password</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_add\_response (uint8\_t \* *res*)

> This API is used to process the response for the add network command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_wmm\_uapsd\_command (int *argc*, char \*\* *argv*)

> This API is used to set the mode of UAPSD (unscheduled automatic power
> save delivery).

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-uapsd-enable</p>
<p>argv[1]: string of UAPSD mode (Required)</p>
<p>0 – disable UAPSD</p>
<p>1 – enable UAPSD.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wmm\_uapsd\_response (uint8\_t \* *res*)

> This API is used to process the response for the set UAPSD command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_POWERMGMT_UAPSD.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_wmm\_uapsd\_qosinfo\_command (int *argc*, char \*\* *argv*)

> This API is used to set/get the QoS (Quality of service) information
> of UAPSD.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1 or 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-uapsd-qosinfo</p>
<p>argv[1]: string of UAPSD QoS Info (Optional)</p>
<p>if is NULL, get the QoS information of UAPSD,</p>
<p>else, set the QoS information of UAPSD.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_uapsd\_qosinfo\_response (uint8\_t \* *res*)

> This API is used to process the response for the set/get UAPSD QoS
> information command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_POWERMGMT_QOSINFO.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_uapsd\_sleep\_period\_command (int *argc*, char \*\* *argv*)

> This API is used to set/get the sleep period of UAPSD.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1 or 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-uapsd-sleep-period</p>
<p>argv[1]: string of UAPSD sleep period (Optional)</p>
<p>if is NULL, get the sleep period of UAPSD,</p>
<p>else, set the sleep period of UAPSD.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_uapsd\_sleep\_period\_response (uint8\_t \* *res*)

> This API is used to process the response for the set/get UAPSD sleep
> period command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_POWERMGMT_SLEEP_PERIOD.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_start\_wps\_pbc\_command (int *argc*, char \*\* *argv*)

> This API is used to start WPS (Wi-Fi protected setup) PBC (push-button
> configuration) to quickly connect to the AP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-start-wps-pbc</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wps\_pbc\_response (uint8\_t \* *res*)

> This API is used to process WPS PBC response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_wps\_generate\_pin\_command (int *argc*, char \*\* *argv*)

> This API is used to generate WPS PIN (personal identification number).

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-generate-wps-pin</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wps\_generate\_pin\_response (uint8\_t \* *res*)

> This API is used to process the response for the generate WPS PIN
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_WPS_GEN_PIN.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_start\_wps\_pin\_command (int *argc*, char \*\* *argv*)

> This API is used to start WPS PIN.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-start-wps-pin</p>
<p>argv[1]: string of WPS PIN (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wps\_pin\_response (uint8\_t \* *res*)

> This API is used to process the response for the start WPS PIN
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_set\_11axcfg\_command (int *argc*, char \*\* *argv*)

> This API is used to modify and set Wi-Fi 802.11ax config.

##### Note

> Implemented as global variable arrays with default config. This API
> can get and change one parameter and it will be restored until reboot.
> Then this config data can be sent to Wi-Fi. Refer to
> NCP\_CMD\_11AX\_CFG\_INFO for config parameter usage and length.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be at least 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-set-11axcfg</p>
<p>argv[1]: action in string (Required) help: show helper dump: dump parameter data currently restored set: set one parameter done: configure current data to Wi-Fi</p>
<p>argv[2]: parameter name (Required when action is "set")</p>
<p>argv[3]: parameter value in hexadecimal, have more argvs when value is more than one byte, like a byte array or uint16_t, uint32_t, uint16_t and uint32_t is ordered in little-endian (Required when action is "set")</p>
<p>...</p>
<p>argv[x]: parameter value in hexadecimal, uint16_t and uint32_t is ordered in little-endian</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_11axcfg\_response (uint8\_t \* *res*)

> This API is used to process Wi-Fi 802.11ax config command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is MCU_NCPCmd_DS_COMMAND with result.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_btwt\_command (int *argc*, char \*\* *argv*)

> This API is used to modify and set Wi-Fi BTWT (broadcast target awake
> time) config.

##### Note

> Implemented as global variable arrays with default config. This API
> can get and change one parameter and it will be restored until reboot.
> Then this config data can be sent to Wi-Fi. Refer to
> NCP\_CMD\_BTWT\_CFG\_INFO for config parameter usage and length.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be at least 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-set-btwt-cfg</p>
<p>argv[1]: action in string (Required) help: show helper dump: dump parameter data currently restored set: set one parameter done: configure current data to Wi-Fi</p>
<p>argv[2]: parameter name (Required when action is "set")</p>
<p>argv[3]: parameter value in hexadecimal, have more argvs when value is more than one byte, like a byte array or uint16_t, uint32_t, uint16_t and uint32_t is ordered in little-endian (Required when action is "set")</p>
<p>...</p>
<p>argv[x]: parameter value in hexadecimal, uint16_t and uint32_t is ordered in little-endian</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_btwt\_response (uint8\_t \* *res*)

> This API is used to process BTWT command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is MCU_NCPCmd_DS_COMMAND with result.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_twt\_setup\_command (int *argc*, char \*\* *argv*)

> This API is used to modify and set TWT (target awake time) config.

##### Note

> Implemented as global variable arrays with default config. This API
> can get and change one parameter and it will be restored until reboot.
> Then this config data can be sent to Wi-Fi. Refer to
> NCP\_CMD\_TWT\_SETUP\_CFG for config parameter usage and length.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be at least 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-twt-setup</p>
<p>argv[1]: action in string (Required) help: show helper dump: dump parameter data currently restored set: set one parameter done: configure current data to Wi-Fi</p>
<p>argv[2]: parameter name (Required when action is "set")</p>
<p>argv[3]: parameter value in hexadecimal, have more argvs when value is more than one byte, like a byte array or uint16_t, uint32_t, uint16_t and uint32_t is ordered in little-endian (Required when action is "set")</p>
<p>...</p>
<p>argv[x]: parameter value in hexadecimal, uint16_t and uint32_t is ordered in little-endian</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_twt\_setup\_response (uint8\_t \* *res*)

> This API is used to process the response for the TWT setup command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is MCU_NCPCmd_DS_COMMAND with result.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_twt\_teardown\_command (int *argc*, char \*\* *argv*)

> This API is used to modify and set TWT teardown config.

##### Note

> Implemented as global variable arrays with default config. This API
> can get and change one parameter and it will be restored until reboot.
> Then this config data can be sent to Wi-Fi. Refer to
> NCP\_CMD\_TWT\_TEARDOWN\_CFG for config parameter usage and length.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be at least 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-twt-teardown</p>
<p>argv[1]: action in string (Required) help: show helper dump: dump parameter data currently restored set: set one parameter done: configure current data to Wi-Fi</p>
<p>argv[2]: parameter name (Required when action is "set")</p>
<p>argv[3]: parameter value in hexadecimal, have more argvs when value is more than one byte, like a byte array or uint16_t, uint32_t, uint16_t and uint32_t is ordered in little-endian (Required when action is "set")</p>
<p>...</p>
<p>argv[x]: parameter value in hexadecimal, uint16_t and uint32_t is ordered in little-endian</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_twt\_teardown\_response (uint8\_t \* *res*)

> This API is used to process the response for the set TWT teardown
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is MCU_NCPCmd_DS_COMMAND with result.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_get\_twt\_report\_command (int *argc*, char \*\* *argv*)

> This API is used to get TWT report information.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-get-twt-report</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_twt\_report\_response (uint8\_t \* *res*)

> This API is used to process the TWT report command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_TWT_REPORT.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_11d\_enable\_command (int *argc*, char \*\* *argv*)

> This API is used to enable or disable Wi-Fi 802.11d feature.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 3.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-set-11d-enable</p>
<p>argv[1]: mode in string, sta: staion, uap: soft AP</p>
<p>argv[2]: state, 0: disable, 1: enable</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_11d\_enable\_response (uint8\_t \* *res*)

> This API is used to process the set 802.11d feature state command
> response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is MCU_NCPCmd_DS_COMMAND with result.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_region\_code\_command (int *argc*, char \*\* *argv*)

> This API is used to set or get the Wi-Fi region code.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2 or 3.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-region-code</p>
<p>argv[1]: action in string, get/set (Required)</p>
<p>argv[2]: region code value to set (Required when action is "set")</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_region\_code\_response (uint8\_t \* *res*)

> This API is used to process the response for the get/set region code
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_REGION_CODE_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_get\_temperature\_command (int *argc*, char \*\* *argv*)

> This API is used to get the device temperature.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-get-temp</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_get\_temperature\_response (uint8\_t \* *res*)

> This API is used to process the response for the get temperature
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_TEMPERATURE.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_time\_command (int *argc*, char \*\* *argv*)

> This API is used to set device time, the time includes year, month,
> day, hour, minute and second.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 7.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-set-time</p>
<p>argv[1]: year</p>
<p>argv[2]: month</p>
<p>argv[3]: day</p>
<p>argv[4]: hour</p>
<p>argv[5]: minute</p>
<p>argv[6]: second</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_get\_time\_command (int *argc*, char \*\* *argv*)

> This API is used to get device time in date.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-get-time</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_time\_response (uint8\_t \* *res*)

> This API is used to process the response for the get time command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_DATE_TIME_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_roaming\_command (int *argc*, char \*\* *argv*)

> This API is used to set soft roaming configuration.
> 
> Call this API to enable/disable soft roaming by specifying the RSSI
> threshold.

##### Note

> **RSSI Threshold setting for soft roaming** : The provided RSSI low
> threshold value is used to subscribe RSSI low event from firmware. On
> reception of this event, background scan is started in firmware with
> same RSSI threshold, to find out APs with better signal strength than
> RSSI threshold.
> 
> If AP is found then roam attempt is initiated, otherwise the
> background scan is started again until limit reaches to
> BG\_SCAN\_LIMIT.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should range from 2 to 3.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-roaming</p>
<p>argv[1]: string value of enable/disable roaming (Required)</p>
<p>0 – disable</p>
<p>1 – enable</p>
<p>argv[2]: string value of RSSI low threshold (Optional)</p>
<p>Default value is 70</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if the call was successful.
> 
> \-WM\_FAIL if failed.

#### int wlan\_process\_roaming\_response (uint8\_t \* *res*)

> This API is used to process the response for the roaming command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_multi\_mef\_command (int *argc*, char \*\* *argv*)

> This API is used to set/delete the MEF (memory efficient filtering)
> entries configuration.

##### Note

> Use this API with command wlan-mcu-sleep with wakeup method MEF. Make
> sure to have connection on STA interface or start UAP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should range from 2 to 3.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-multi-mef</p>
<p>argv[1]: Packet type string (Required)</p>
<p>ping -– Add MEF entry for ping packets</p>
<p>arp -– Add MEF entry for ARP packets</p>
<p>multicast -– Add MEF entry for multicast packets</p>
<p>ns -– Add MEF entry for IPV6 NS packets</p>
<p>del -– Delete all MEF entries</p>
<p>argv[2]: Sting value of action (Required)</p>
<p>0 –- Discard and not wakeup host</p>
<p>1 –- Discard and wakeup host</p>
<p>3 –- Allow and wakeup host</p></td>
</tr>
</tbody>
</table>

##### Note

> argv\[2\] is not needed if argv\[1\] is del.

##### Returns

> WM\_SUCCESS if the call was successful.
> 
> \-WM\_FAIL if failed.

#### int wlan\_process\_multi\_mef\_response (uint8\_t \* *res*)

> This API is used to process multi MEF response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_wakeup\_condition\_command (int *argc*, char \*\* *argv*)

> This API is used to configure Wi-Fi wakeup conditions. Once any
> condition is meet, NCP device will be woken up by Wi-Fi. Some of the
> wakeup conditions needs connection on station or uAP.

##### Note

> Use this API with wlan\_multi\_mef\_command if MEF mode is used.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should range from 2 to 3.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-wakeup-condition</p>
<p>argv[1]: String of wakeup mode (Required)</p>
<p>wowlan –- Default wakeup conditions</p>
<p>mef –- MEF conditions</p>
<p>argv[2]: String of wakeup bitmap for mode wowlan</p>
<p>bit[0] – WAKE_ON_ALL_BROADCAST</p>
<p>bit[1] – WAKE_ON_UNICAST</p>
<p>bit[2] – WAKE_ON_MAC_EVENT</p>
<p>bit[3] – WAKE_ON_MULTICAST</p>
<p>bit[4] – WAKE_ON_ARP_BROADCAST</p>
<p>bit[6] – WAKE_ON_MGMT_FRAME</p></td>
</tr>
</tbody>
</table>

##### Note

> The argv\[2\] is only required when argv\[1\] is wowlan.
> 
> If argv\[1\] is wowlan and all bits of argv\[2\] are 0, NCP device
> won't be woken up by Wi-Fi.

##### Returns

> WM\_SUCCESS if the call was successful.
> 
> \-WM\_FAIL if failed.

#### int wlan\_process\_wakeup\_condition\_response (uint8\_t \* *res*)

> This API is used to process the response for the configure Wi-Fi
> wakeup condition command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_suspend\_command (int *argc*, char \*\* *argv*)

> This API is used to request NCP device enter specific low power mode.

##### Note

> Should be used after API ncp\_mcu\_sleep\_command with manual mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-suspend</p>
<p>argv[1]: String value of low power mode</p>
<p>1 – PM1</p>
<p>2 – PM2</p>
<p>3 – PM3</p></td>
</tr>
</tbody>
</table>

##### Note

> The allowed low power mode is controlled by configuration of
> ncp\_wake\_cfg\_command.
> 
> For INTF wake mode, the allowed \<power\_mode\> are PM1 and PM2.
> 
> For GPIO wake mode and RDRW612 as NCP device, the allowed
> \<power\_mode\> are PM1, PM2 and PM3.
> 
> For GPIO wake mode and FRDMRW612 as NCP device, the allowed
> \<power\_mode\> are PM1 and PM2.
> 
> For WIFI-NB wake mode and FRDRW612 as NCP device, the allowed
> \<power\_mode\> are PM1, PM2 and PM3.

##### Returns

> WM\_SUCCESS if the call was successful.
> 
> \-WM\_FAIL if failed.

#### int wlan\_process\_suspend\_response (uint8\_t \* *res*)

> This API is used to process suspend response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_deep\_sleep\_ps\_command (int *argc*, char \*\* *argv*)

> This API is used to enable/disable deep sleep power save mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-deep-sleep-ps</p>
<p>argv[1]: enable/disable deep sleep power save mode.</p>
<p>0 – disable deep sleep</p>
<p>1 – enable deep sleep</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Deep sleep power save is enabled by default.

#### int wlan\_process\_deep\_sleep\_ps\_response (uint8\_t \* *res*)

> This API is used to process the response for the deep sleep power save
> mode command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_ieee\_ps\_command (int *argc*, char \*\* *argv*)

> This API is used to enable/disable ieee power save mode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of strings pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: wlan-ieee-ps</p>
<p>argv[1]: enable/disable ieee power save mode.</p>
<p>0 – disable ieee power save mode</p>
<p>1 – enable ieee power save mode</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

##### Note

> Ieee power save is enabled by default.

#### int wlan\_process\_ieee\_ps\_response (uint8\_t \* *res*)

> This API is used to process the response for the enable/disable ieee
> power save mode command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS

#### int wlan\_socket\_open\_command (int *argc*, char \*\* *argv*)

> This API is used to create a socket.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 2 to 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-open</p>
<p>argv[1]: string value of socket_type (Required)</p>
<p>tcp – TCP socket</p>
<p>udp – UDP socket</p>
<p>raw – raw socket</p>
<p>argv[2]: string value of socket domain (Optional)</p>
<p>ipv4 – default is ipv4 domain</p>
<p>ipv6 – ipv6 domain</p>
<p>argv[3]: string value of socket procotol (Optional)</p>
<p>icmp – default is icmp protocol</p>
<p>icmpv6 – icmpv6 protocol</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_open\_response (uint8\_t \* *res*)

> This API is used to process the socket open command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_SOCKET_OPEN_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_con\_command (int *argc*, char \*\* *argv*)

> This API is used to connect peer socket.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 2 to 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-connect</p>
<p>argv[1]: string value of peer socket fd to connect (Required)</p>
<p>argv[2]: string value of peer socket ipv4 address to connect, so far only support IPV4 address (Required)</p>
<p>argv[3]: string value of peer socket port (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_con\_response (uint8\_t \* *res*)

> This API is used to process the socket connect command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_bind\_command (int *argc*, char \*\* *argv*)

> This API is used to bind socket.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 2 to 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-bind</p>
<p>argv[1]: string value of socket fd to bind (Required)</p>
<p>argv[2]: string value of socket ipv4 address to connect, so far only support IPV4 address (Required)</p>
<p>argv[3]: string value of socket port (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_bind\_response (uint8\_t \* *res*)

> This API is used to process socket bind command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_close\_command (int *argc*, char \*\* *argv*)

> This API is used to close the opened socket.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-close</p>
<p>argv[1]: string value of opened socket fd to close (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_close\_response (uint8\_t \* *res*)

> This API is used to process the response for the socket close command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_listen\_command (int *argc*, char \*\* *argv*)

> This API is used to listen TCP client's connection.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 3.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-listen</p>
<p>argv[1]: string value of opened socket fd to listen (Required)</p>
<p>argv[2]: string value of listen maximum number (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_listen\_response (uint8\_t \* *res*)

> This API is used to process the response for the socket listen
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_accept\_command (int *argc*, char \*\* *argv*)

> This API is used to accept the TCP server's connection.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2,</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-accept</p>
<p>argv[1]: string value of TCP server listen socket fd (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_accept\_response (uint8\_t \* *res*)

> This API is used to process the response for the socket accept
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_SOCKET_ACCEPT_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_send\_command (int *argc*, char \*\* *argv*)

> This API is used to send data.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 3 to 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-send</p>
<p>argv[1]: string value of socket fd, use the socket to send data (Required)</p>
<p>argv[2]: string value of send data , the max send data buffer is 4076. (Required)</p>
<p>argv[3]: string value of send data size, we can specify send data size (Optional)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_send\_response (uint8\_t \* *res*)

> This API is used to process socket send command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_sendto\_command (int *argc*, char \*\* *argv*)

> This API is used to send data to specified IP and port.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 6 to 7.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-sendto</p>
<p>argv[1]: int value of socket fd, use the socket to send data (Required)</p>
<p>argv[2]: string value of peer ip addr, the max length is 16 (Required)</p>
<p>argv[3]: string value of peer socket port (Required)</p>
<p>argv[4]: string value of send data, the max length is 4056 (Required)</p>
<p>argv[5]: string value of send data size, we can specify send data size (Optional)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_sendto\_response (uint8\_t \* *res*)

> This API is used to process the socket send command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response,</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_receive\_command (int *argc*, char \*\* *argv*)

> This API is used to receive data from socket.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-receive</p>
<p>argv[1]: string value of socket fd, receive data from the socket fd (Required)</p>
<p>argv[2]: string value of receive size, the buffer max length is 4072 (Required)</p>
<p>argv[3]: string value of wait time, return fail when timemout (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_receive\_response (uint8\_t \* *res*)

> This API is used to process the socket receive command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_SOCKET_RECEIVE_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_socket\_recvfrom\_command (int *argc*, char \*\* *argv*)

> This API is used to receive data from socket and return peer socket ip
> and port.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-socket-recvfrom</p>
<p>argv[1]: string value of socket fd, receive data from the socket fd (Required)</p>
<p>argv[2]: string value of receive size, the buffer max length is 4072 (Required)</p>
<p>argv[3]: string value of wait time, return fail when timemout (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_socket\_recvfrom\_response (uint8\_t \* *res*)

> This API is used to process the socket recvfrom command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_SOCKET_RECVFROM_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_http\_connect\_command (int *argc*, char \*\* *argv*)

> This API is used to connect the HTTP server.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 3.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-http-connect</p>
<p>argv[1]: string value of http handle to connect, (Required)</p>
<p>argv[2]: string value of http server host address, the string max length is 512 (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_http\_con\_response (uint8\_t \* *res*)

> This API is used to process the HTTP connect command response.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_HTTP_CON_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_http\_disconnect\_command (int *argc*, char \*\* *argv*)

> This API is used to disconnect the HTTP server.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 2.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-http-disconnect</p>
<p>argv[1]: string value of http handle to disconnect, (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_http\_discon\_response (uint8\_t \* *res*)

> This API is used to process the response for the HTTP disconnect
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_http\_req\_command (int *argc*, char \*\* *argv*)

> This API is used to send request to the HTTP server.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 4 to 6.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-http-req</p>
<p>argv[1]: int value of http handle to send http request (Required)</p>
<p>argv[2]: string value of http method, the max length is 16 (Required)</p>
<p>get – http request get method</p>
<p>delete – http request delete method</p>
<p>put – http request put method</p>
<p>options – http request options method</p>
<p>argv[3]: string value of http URL, the string max length is 512 (Required)</p>
<p>argv[4]: string value of http send data, the buffer max length is 3560 (Optional)</p>
<p>argv[5]: string value of http send data size (Optional)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_http\_req\_response (uint8\_t \* *res*)

> This API is used to process the response for the HTTP request.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_HTTP_REQ_RESP_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_http\_recv\_command (int *argc*, char \*\* *argv*)

> This API is used to receive the HTTP server data.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-http-receive</p>
<p>argv[1]: string value of http handle to receive http data (Required)</p>
<p>argv[2]: string value of http received data, the buffer max length is 4072. (Required)</p>
<p>argv[3]: string value of http wait time, return fail when timemout (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure

#### int wlan\_process\_wlan\_http\_recv\_response (uint8\_t \* *res*)

> This API is used to process the response for the HTTP receive command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_HTTP_RECV_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_http\_unseth\_command (int *argc*, char \*\* *argv*)

> This API is used to unset the HTTP request header.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-http-unseth</p>
<p>argv[1]: string http header segment name, the max length is 64 (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_http\_unseth\_response (uint8\_t \* *res*)

> This API is used to process the response for the HTTP unset header
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_http\_seth\_command (int *argc*, char \*\* *argv*)

> This API is used to set the HTTP request header.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-http-seth</p>
<p>argv[1]: string value of segment name of the header, the max length is 64 (Required)</p>
<p>argv[2]: string value of segment value of the header, the max length is 128 (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_http\_seth\_response (uint8\_t \* *res*)

> This API is used to process the response for the set HTTP header
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_websocket\_upg\_command (int *argc*, char \*\* *argv*)

> This API is used to upgrade the HTTP to websocket.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-websocket-upg</p>
<p>argv[1]: string value of http handle (Required)</p>
<p>argv[2]: string value of websocket URI (Required)</p>
<p>/ws/ – websocket uri is /ws/</p>
<p>argv[3]: string value of websocket type (Required)</p>
<p>echo – websocket type is echo</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_websocket\_upg\_response (uint8\_t \* *res*)

> This API is used to process the response for the websocket upgrade
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success otherwise return -WM\_FAIL.

#### int wlan\_websocket\_send\_command (int *argc*, char \*\* *argv*)

> This API is used to send data to the websocket server.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 4 to 5.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-websocket-send</p>
<p>argv[1]: string value of http handle upgraded to websocket (Required)</p>
<p>argv[2]: string value of websocket type (Required)</p>
<p>text – websocket text type data</p>
<p>bin – websocket bin type data</p>
<p>argv[3]: string value of send data, the buffer max length is 4060. (Required)</p>
<p>argv[4]: string value of send data size (Optional)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_websocket\_send\_response (uint8\_t \* *res*)

> This API is used to process the response for the websocket send data
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NULL.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_websocket\_recv\_command (int *argc*, char \*\* *argv*)

> This API is used to receive data from the websocket server.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 4.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-websocket-recv</p>
<p>argv[1]: string value of http handle to receive websocket data (Required)</p>
<p>argv[2]: string value of websocket received data, the buffer max length is 4068. (Required)</p>
<p>argv[3]: string value of websocket wait time, return fail when timemout (Required)</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_wlan\_websocket\_recv\_response (uint8\_t \* *res*)

> This API is used to process the response for the websocket receive
> data command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body is NCP_CMD_WEBSOCKET_RECV_CFG.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### void ncp\_network\_commissioning (ncp\_commission\_cfg\_t \* *ncp\_commission\_cfg*)

> This API is used to the Wi-Fi receive commission information from
> Bluetooth LE.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>ncp_commission_cfg</em></td>
<td>A pointer to ncp_commission_cfg_t from Bluetooth LE.</td>
</tr>
</tbody>
</table>

#### int wlan\_11k\_neighbor\_req\_command (int *argc*, char \*\* *argv*)

> This API is used to send the neighbor report request.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 1 or 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-11k-neigbor-req".</p>
<p>argv[1]: string value of the SSID for neighbor report (Required).</p></td>
</tr>
</tbody>
</table>

##### Note

> ssid parameter is optional

##### Returns

> WM\_SUCCESS if successful.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_neighbor\_req\_response (uint8\_t \* *res*)

> This API is used to process the response of the send neighbor report
> request.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_11k\_cfg\_command (int *argc*, char \*\* *argv*)

> This API is used to enable or disable the 802.11k feature in Wi-Fi
> firmware.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-11k-enable".</p>
<p>argv[1]: string value of enable or disable 802.11k feature.</p>
<p>0: disable 802.11k feature.</p>
<p>1: enable 802.11k feature.</p></td>
</tr>
</tbody>
</table>

##### Note

> 802.11k is disabled by default.

##### Returns

> WM\_SUCCESS if successful.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_11k\_cfg\_response (uint8\_t \* *res*)

> This API is used to process the command response of enable or disable
> 802.11k feature command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_csi\_cfg\_command (int *argc*, char \*\* *argv*)

> This API is used to set the CSI configuration.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-csi-cfg".</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_csi\_response (uint8\_t \* *res*)

> This API is used to process the response for the set CSI configuration
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_set\_csi\_param\_header\_command (int *argc*, char \*\* *argv*)

> This API is used to set CSI header part of parameters.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 10.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-set-csi-param-header".</p>
<p>argv[1]: string value of bss type</p>
<p>"sta": bss type is station</p>
<p>"uap": bss type is UAP.</p>
<p>argv[2]: string value of enable or disable csi.</p>
<p>1: enable csi</p>
<p>0: disable csi</p>
<p>argv[3]: string value of head id</p>
<p>head id is symbol placed at start of csi data</p>
<p>used to separate CSI event records received from FW, could be set as any value.</p>
<p>argv[4]: string value of tail id</p>
<p>tail id is symbol placed at end of csi data</p>
<p>used to separate CSI event records received from FW, could be set as any value.</p>
<p>argv[5]: string value of chip id</p>
<p>used to separate CSI event records received from FW, could be set as any value.</p>
<p>argv[6]: band configure</p>
<p>properties of csi channel</p>
<p>bit[0:1]: Band Info</p>
<p>00: 2.4GHz</p>
<p>01: 5GHz</p>
<p>bit[2:3]: chan Width</p>
<p>00: 20MHz</p>
<p>10: 40MHz</p>
<p>11: 80MHz</p>
<p>bit[4:5]: chan to Offset</p>
<p>00: None</p>
<p>10: Above</p>
<p>11: Below</p>
<p>bit[6:7]: scan Mode</p>
<p>00: manual</p>
<p>10: ACS.</p>
<p>argv[7]: string value of channel number.</p>
<p>argv[8]: string value of enable or disable csi monitor feature</p>
<p>1: enable csi monitor</p>
<p>0: disable csi monitor.</p>
<p>argv[9]: string value of enable or disable csi ra for us feature</p>
<p>1: enable csi ra for us</p>
<p>0: disable csi ra for us.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_csi\_filter\_command (int *argc*, char \*\* *argv*)

> This API is used to set the filter for CSI.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of string pointed to by argv, argc should be 2 or 6.</p>
<p>2: action for filter is delete, clear and dump</p>
<p>6: action for filter is add.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-set-csi-filter".</p>
<p>argv[1]: string value of action for filter</p>
<p>"add": add one filter to CSI filter</p>
<p>"delet": delete last filter entry of CSI filter</p>
<p>"clear": clear all entries of CSI filter</p>
<p>"dump": dump all entries of CSI filter.</p>
<p>argv[2]: string value of mac address, format like 11:22:33:44:55:66.</p>
<p>argv[3]: string value of 802.11 frame control filed type.</p>
<p>argv[4]: string value of 802.11 frame control filed subtype.</p>
<p>argv[5]: string value of flag</p>
<p>bit[0]: reserved, must be set to 0</p>
<p>bit[1]: wait for trigger, not implement currently, must be set to 0</p>
<p>bit[2]: send CSI error event when timeout, not implement currently, must be set to 0.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_net\_monitor\_cfg\_command (int *argc*, char \*\* *argv*)

> This API is used to set the monitor configuration.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-net-monitor-cfg"</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_monitor\_response (uint8\_t \* *res*)

> This API is used to process the response of the set monitor
> configuration command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body: None.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_set\_monitor\_filter\_command (int *argc*, char \*\* *argv*)

> This API is used to set the filter for monitor.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of string pointed to by argv, argc should be 2 or 3.</p>
<p>2: action for filter is delete, clear and dump</p>
<p>3: action for filter is add.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-set-monitor-filter".</p>
<p>argv[1]: string value of action for filter</p>
<p>"add": add one filter to monitor filter</p>
<p>"delet": delete last filter entry of monitor filter</p>
<p>"clear": clear all entries of monitor filter</p>
<p>"dump": dump all entries of monitor filter.</p>
<p>argv[2]: string value of mac address, format like 11:22:33:44:55:66</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_set\_monitor\_param\_command (int *argc*, char \*\* *argv*)

> This API is used to set the monitor parameters.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 6.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-set-monitor-param".</p>
<p>argv[1]: string value of action</p>
<p>1: action set.</p>
<p>argv[2]: string value of enable or disable monitor</p>
<p>1: enable monitor</p>
<p>other values: disable monitor.</p>
<p>argv[3]: string value of filter flags</p>
<p>bit[0]: enable or disable management frame</p>
<p>1: enable management frame</p>
<p>0: disable management frame</p>
<p>bit[1]: enable or disable control frame</p>
<p>1: enable control frame</p>
<p>0: disable control frame</p>
<p>bit[2]: enable or disable data frame</p>
<p>1: enable data frame</p>
<p>0: disable data frame.</p>
<p>argv[4]: string value of radio type</p>
<p>properties of monitor channel</p>
<p>bit[0:1]: Band Info</p>
<p>00: 2.4GHz</p>
<p>01: 5GHz</p>
<p>bit[2:3]: chan Width</p>
<p>00: 20MHz</p>
<p>10: 40MHz</p>
<p>11: 80MHz</p>
<p>bit[4:5]: chan to Offset</p>
<p>00: None</p>
<p>10: Above</p>
<p>11: Below</p>
<p>bit[6:7]: scan Mode</p>
<p>00: manual</p>
<p>10: ACS.</p>
<p>argv[5]: string value of channel number</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_address\_command (int *argc*, char \*\* *argv*)

> This API is used to get IP address of the station.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should be 1.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-address</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.

#### int wlan\_process\_address (uint8\_t \* *res*)

> This API is used to process the response for the get STA IP address
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_NETWORK_ADDRESS.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_start\_network\_command (int *argc*, char \*\* *argv*)

> This API is used to start a UAP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-start-network".</p>
<p>argv[1]: string value of network name.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if successful.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_start\_network\_response (uint8\_t \* *res*)

> This API is used to process the response for the start UAP network
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td><p>A pointer to MCU_NCPCmd_DS_COMMAND response.</p>
<p>Response body refer to NCP_CMD_NETWORK_START.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if UAP starts successfully.
> 
> \-WM\_FAIL if failure.

#### int wlan\_stop\_network\_command (int *argc*, char \*\* *argv*)

> This API is used to stop a UAP network.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-stop-netwok".</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_process\_stop\_network\_response (uint8\_t \* *res*)

> This API is used to process the response for the stop UAP network
> command.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_get\_uap\_sta\_list\_command (int *argc*, char \*\* *argv*)

> This API is used to get the information for all STAs those connected
> to an AP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-get-uap-sta-list".</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if UAP starts successfully.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_get\_uap\_sta\_list (uint8\_t \* *res*)

> This API is used to process the information for all STAs those
> connected to an AP.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_NETWORK_UAP_STA_LIST.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS.

#### int wlan\_list\_command (int *argc*, char \*\* *argv*)

> This API is used to get all the networks information those added
> locally.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 1.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-list".</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_network\_list\_response (uint8\_t \* *res*)

> This API is used to process information of all the network those added
> locally.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_NETWORK_LIST.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_remove\_command (int *argc*, char \*\* *argv*)

> This API is used to remove an AP from the local network list.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td>Argument count, the number of string pointed to by argv, argc should be 2.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector.</p>
<p>argv[0]: should be "wlan-remove".</p>
<p>argv[1]: string value of network name.</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if successful.
> 
> \-WM\_FAIL if failure.

#### int wlan\_process\_network\_remove\_response (uint8\_t \* *res*)

> This API is used to process the removed AP information.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>res</em></td>
<td>A pointer to MCU_NCPCmd_DS_COMMAND response. Response body refer to NCP_CMD_NETWORK_REMOVE.</td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure.

#### int wlan\_ncp\_iperf\_command (int *argc*, char \*\* *argv*)

> This is a private NCP throughput test API that needs to play with
> sample TCP/UDP server/client, and could not play with open source
> iperf.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>argc</em></td>
<td><p>Argument count, the number of strings pointed to by argv,</p>
<p>argc should range from 5 to 9.</p></td>
</tr>
<tr class="even">
<td>in</td>
<td><em>argv</em></td>
<td><p>Argument vector,</p>
<p>argv[0]: wlan-ncp-iperf</p>
<p>argv[1]: string value of socket handle index opended (Required)</p>
<p>argv[2]: string value of protocol type, "tcp" or "udp". (Required)</p>
<p>argv[3]: string value of data direction, "tx"(send) or "rx"(receive) (Required)</p>
<p>argv[4]: string value of packet count for TCP or string value of peer IP address for UDP (Required)</p>
<p>argv[5]: string value of peer port number for UDP (Required)</p>
<p>argv[6]: string value of packet count except to receive or send for UDP (Required)</p>
<p>argv[7]: string value of data rate for UDP (Required)</p>
<p>argv[8]: string value of duration for UDP (Optional).</p></td>
</tr>
</tbody>
</table>

##### Returns

> WM\_SUCCESS if success.
> 
> \-WM\_FAIL if failure

### Macro Documentation

#### \#define MLAN\_MAX\_VER\_STR\_LEN  128

> Maximum length of firmware software version string.

#### \#define MLAN\_MAC\_ADDR\_LENGTH  6

> MAC address length.

#### \#define WLAN\_NETWORK\_NAME\_MAX\_LENGTH  32

> Maximum length of network name.
> 
> The space reserved for storing network names

#### \#define IDENTITY\_MAX\_LENGTH  64

> Maximum length of identity string .

#### \#define PASSWORD\_MAX\_LENGTH  128

> Maximum length of password string .

#### \#define MAX\_NUM\_CLIENTS  16

> Maximum number of STAs are connected to the UAP.

#### \#define CSI\_FILTER\_MAX  16

> Maximum CSI filter count

#### \#define NCP\_IPERF\_END\_TOKEN\_SIZE  11

> Size of iperf end token.

#### \#define WLAN\_NETWORK\_NAME\_MAX\_LENGTH  32

> Maximum length of network name.
> 
> The space reserved for storing network names

#### \#define DNS\_RRTYPE\_A  1

> A host address

#### \#define DNS\_RRTYPE\_PTR  12

> A domain name pointer

#### \#define WLAN\_PASSWORD\_MAX\_LENGTH  255

> Wi-Fi password maximum length

### Enumeration Type Documentation

#### enum wlan\_bss\_role

> Network wireless BSS role.

##### Enumerator:

<table>
<tbody>
<tr class="odd">
<td>WLAN_BSS_ROLE_STA</td>
<td>Infrastructure network. The system will act as a station connected to an Access Point.</td>
</tr>
<tr class="even">
<td>WLAN_BSS_ROLE_UAP</td>
<td>UAP network. The system will act as an UAP node to which other Wireless clients can connect.</td>
</tr>
<tr class="odd">
<td>WLAN_BSS_ROLE_ANY</td>
<td>Either infrastructure network or UAP network</td>
</tr>
</tbody>
</table>

#### enum wlan\_security\_type

> WLAN network security type

##### Enumerator:

<table>
<tbody>
<tr class="odd">
<td>WLAN_SECURITY_NONE</td>
<td>The network does not use security.</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_WEP_OPEN</td>
<td>The network uses WEP (wired equivalent privacy) security with open key.</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_WEP_SHARED</td>
<td>The network uses WEP security with shared key.</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_WPA</td>
<td>The network uses WPA (Wi-Fi protected access) security with PSK.</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_WPA2</td>
<td>The network uses WPA2 (Wi-Fi protected access 2) security with PSK.</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_WPA_WPA2_MIXED</td>
<td>The network uses WPA/WPA2 mixed security with PSK</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_WPA2_FT</td>
<td>The network uses WPA2 security with PSK (pre-shared key) FT (fast BSS transition).</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_WPA3_SAE</td>
<td>The network uses WPA3 security with SAE (simultaneous authentication of equals).</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_WPA3_FT_SAE</td>
<td>The network uses WPA3 security with SAE FT.</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_WPA3_SAE_EXT_KEY</td>
<td>The network uses WPA3 security with SAE EXT KEY.</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_WPA2_WPA3_SAE_MIXED</td>
<td>The network uses WPA2/WPA3 SAE mixed security with PSK. This security mode is specific to uAP or SoftAP only</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_OWE_ONLY</td>
<td>The network uses OWE only security without transition mode support.</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_EAP_TLS</td>
<td>The network uses WPA2 Enterprise EAP-TLS (extensible authentication protocol transport layer security). The identity field in wlan_network structure is used</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_EAP_TLS_SHA256</td>
<td>The network uses WPA2 enterprise EAP-TLS SHA256 security The identity field in wlan_network structure is used</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_EAP_TLS_FT</td>
<td>The network uses WPA2 Enterprise EAP-TLS FT security The identity field in wlan_network structure is used</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_EAP_TLS_FT_SHA384</td>
<td>The network uses WPA2 Enterprise EAP-TLS FT SHA384 security The identity field in wlan_network structure is used</td>
</tr>
<tr class="odd">
<td>WLAN_SECURITY_EAP_PEAP_MSCHAPV2</td>
<td>The network uses WPA2 Enterprise EAP-PEAP-MSCHAPV2 security The anonymous identity, identity and password fields in wlan_network structure are used</td>
</tr>
<tr class="even">
<td>WLAN_SECURITY_WILDCARD</td>
<td>The network can use any security method. This is often used when the user only knows the name and passphrase but not the security type.</td>
</tr>
</tbody>
</table>

#### anonymous enum

##### Enumerator:

<table>
<tbody>
<tr class="odd">
<td>ADDR_TYPE_STATIC</td>
<td>static IP address</td>
</tr>
<tr class="even">
<td>ADDR_TYPE_DHCP</td>
<td>Dynamic IP address</td>
</tr>
<tr class="odd">
<td>ADDR_TYPE_LLA</td>
<td>Link level address</td>
</tr>
</tbody>
</table>

#### enum wlan\_ps\_mode

> Station power save mode.

##### Enumerator:

<table>
<tbody>
<tr class="odd">
<td>WLAN_ACTIVE</td>
<td>Active mode.</td>
</tr>
<tr class="even">
<td>WLAN_IEEE</td>
<td>IEEE power save mode.</td>
</tr>
<tr class="odd">
<td>WLAN_DEEP_SLEEP</td>
<td>Deep sleep power save mode.</td>
</tr>
<tr class="even">
<td>WLAN_IEEE_DEEP_SLEEP</td>
<td>IEEE deep sleep power save mode.</td>
</tr>
<tr class="odd">
<td>WLAN_WNM</td>
<td>WNM power save mode.</td>
</tr>
<tr class="even">
<td>WLAN_WNM_DEEP_SLEEP</td>
<td>WNM deep sleep power save mode.</td>
</tr>
</tbody>
</table>

#### enum wlan\_connection\_state

> Wi-Fi station/UAP/Wi-Fi direct connection/status state.

##### Enumerator:

<table>
<tbody>
<tr class="odd">
<td>WLAN_DISCONNECTED</td>
<td>The Wi-Fi connection manager is not connected and no connection attempt is in progress. It is possible to connect to a network or scan.</td>
</tr>
<tr class="even">
<td>WLAN_CONNECTING</td>
<td>The Wi-Fi connection manager is not connected but it is currently attempting to connect to a network. It is not possible to scan at this time. It is possible to connect to a different network.</td>
</tr>
<tr class="odd">
<td>WLAN_ASSOCIATED</td>
<td>The Wi-Fi connection manager is not connected but associated.</td>
</tr>
<tr class="even">
<td>WLAN_AUTHENTICATED</td>
<td>The Wi-Fi connection manager is not connected but authenticated.</td>
</tr>
<tr class="odd">
<td>WLAN_CONNECTED</td>
<td>The Wi-Fi connection manager is connected. It is possible to scan and connect to another network at this time. Information about the current network configuration is available.</td>
</tr>
<tr class="even">
<td>WLAN_UAP_STARTED</td>
<td>The Wi-Fi connection manager has started UAP</td>
</tr>
<tr class="odd">
<td>WLAN_UAP_STOPPED</td>
<td>The Wi-Fi connection manager has stopped UAP</td>
</tr>
<tr class="even">
<td>WLAN_SCANNING</td>
<td>The Wi-Fi connection manager is not connected and network scan is in progress.</td>
</tr>
<tr class="odd">
<td>WLAN_ASSOCIATING</td>
<td>The Wi-Fi connection manager is not connected and network association is in progress.</td>
</tr>
</tbody>
</table>

#### 

## otopcode.h File Reference

This file provides interface to get Open Thread commands corresponding
opcode.

### Function Documentation

#### int8\_t ot\_get\_opcode (uint8\_t \* *userinputcmd*, uint8\_t *otcmdlen*)

> Get OpenThread commands corresponding opcode. This function search OT
> command list to find command's opcode.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>userinputcmd</em></td>
<td>user input OT command.</td>
</tr>
<tr class="even">
<td>in</td>
<td><em>otcmdlen</em></td>
<td>OT command length.</td>
</tr>
</tbody>
</table>

##### Returns

> opcode of the command.

#### 

## service.h File Reference

Bluetooth service definitions.

### Function Documentation

#### static void sys\_put\_le16 (uint16\_t *val*, uint8\_t *dst*\[2\])\[inline\], \[static\]

> Put a 16-bit integer as little-endian to arbitrary location.
> 
> Put a 16-bit integer, originally in host endianness, to a potentially
> unaligned memory location in little-endian format.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>val</em></td>
<td>16-bit integer in host endianness.</td>
</tr>
<tr class="even">
<td>in,out</td>
<td><em>dst</em></td>
<td>Destination memory address to store the result.</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### static void sys\_put\_le32 (uint32\_t *val*, uint8\_t *dst*\[4\])\[inline\], \[static\]

> Put a 32-bit integer as little-endian to arbitrary location.
> 
> Put a 32-bit integer, originally in host endianness, to a potentially
> unaligned memory location in little-endian format.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>val</em></td>
<td>32-bit integer in host endianness.</td>
</tr>
<tr class="even">
<td>in,out</td>
<td><em>dst</em></td>
<td>Destination memory address to store the result.</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### static void sys\_put\_le24 (uint32\_t *val*, uint8\_t *dst*\[3\])\[inline\], \[static\]

> Put a 24-bit integer as little-endian to arbitrary location.
> 
> Put a 24-bit integer, originally in host endianness, to a potentially
> unaligned memory location in little-endian format.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in</td>
<td><em>val</em></td>
<td>24-bit integer in host endianness.</td>
</tr>
<tr class="even">
<td>in,out</td>
<td><em>dst</em></td>
<td>Destination memory address to store the result.</td>
</tr>
</tbody>
</table>

##### Returns

> void

#### static uint16\_t sys\_get\_le16 (const uint8\_t *src*\[2\])\[inline\], \[static\]

> Get a 16-bit integer stored in little-endian format.
> 
> Get a 16-bit integer, stored in little-endian format in a potentially
> unaligned memory location, and convert it to the host endianness.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in,out</td>
<td><em>src</em></td>
<td>Location of the little-endian 16-bit integer to get.</td>
</tr>
</tbody>
</table>

##### Returns

> 16-bit integer in host endianness.

#### static uint32\_t sys\_get\_le32 (const uint8\_t *src*\[4\])\[inline\], \[static\]

> Get a 32-bit integer stored in little-endian format.
> 
> Get a 32-bit integer, stored in little-endian format in a potentially
> unaligned memory location, and convert it to the host endianness.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in,out</td>
<td><em>src</em></td>
<td>Location of the little-endian 32-bit integer to get.</td>
</tr>
</tbody>
</table>

##### Returns

> 32-bit integer in host endianness.

#### static uint32\_t sys\_get\_le24 (const uint8\_t *src*\[3\])\[inline\], \[static\]

> Get a 24-bit integer stored in little-endian format.
> 
> Get a 24-bit integer, stored in little-endian format in a potentially
> unaligned memory location, and convert it to the host endianness.

##### Parameters

<table>
<tbody>
<tr class="odd">
<td>in,out</td>
<td><em>src</em></td>
<td>Location of the little-endian 24-bit integer to get.</td>
</tr>
</tbody>
</table>

##### Returns

> 24-bit integer in host endianness.

### Macro Documentation

#### \#define NCP\_BLE\_SERVICE\_PRIO  0

> NCP Bluetooth LE priority

#### \#define BT\_BYTES\_LIST\_LE16( \_v)

**Value:** (((\_v) \>\> 0) & 0xFFU), \\

(((\_v) \>\> 8) & 0xFFU) \\

> NCP Bluetooth LE inline tool function

#### \#define HOST\_GATT\_ATTRIBUTE( \_type, \_uuid, \_props, \_perm)

**Value:**{ \\

.type = \_type, \\

.uuid = {BT\_BYTES\_LIST\_LE16(\_uuid)}, \\

.uuid\_length = BT\_UUID\_SIZE\_16, \\

.properties = \_props, \\

.permissions = \_perm, \\

}

> NCP host set gatt attribute configuration

#### \#define NCP\_BLE\_HOST\_SERVICE\_AUTO\_START

> Auto register host services into ncp device database

#### \#define BT\_GATT\_CHRC\_BROADCAST  0x01

> Characteristic Properties Bit field values Bluetooth GATT
> Characteristic broadcast property.
> 
> If set, permits broadcasts of the Characteristic Value using Server
> Characteristic Configuration Descriptor.

#### \#define BT\_GATT\_CHRC\_READ  0x02

> Bluetooth GATT Characteristic read property.
> 
> If set, permits reads of the Characteristic Value.

#### \#define BT\_GATT\_CHRC\_WRITE\_WITHOUT\_RESP  0x04

> Bluetooth GATT Characteristic write without response property.
> 
> If set, permits write of the Characteristic Value without response.

#### \#define BT\_GATT\_CHRC\_WRITE  0x08

> Bluetooth GATT Characteristic write with response property.
> 
> If set, permits write of the Characteristic Value with response.

#### \#define BT\_GATT\_CHRC\_NOTIFY  0x10

> Bluetooth GATT Characteristic notify property.
> 
> If set, permits notifications of a Characteristic Value without
> acknowledgment.

#### \#define BT\_GATT\_CHRC\_INDICATE  0x20

> Bluetooth GATT Characteristic indicate property.
> 
> If set, permits indications of a Characteristic Value with
> acknowledgment.

#### \#define BT\_GATT\_CHRC\_AUTH  0x40

> Bluetooth GATT Characteristic Authenticated Signed Writes property.
> 
> If set, permits signed writes to the Characteristic Value.

#### \#define BT\_GATT\_CHRC\_EXT\_PROP  0x80

> Bluetooth GATT Characteristic Extended Properties.
> 
> If set, additional characteristic properties are defined in the
> Characteristic Extended Properties Descriptor.

#### \#define BT\_GATT\_PERM\_NONE  0x0000

> GATT attribute permission bit field values No operations supported,
> e.g. for notify-only

#### \#define BT\_GATT\_PERM\_READ  0x0001

> Attribute read permission.

#### \#define BT\_GATT\_PERM\_WRITE  0x0002

> Attribute write permission.

#### \#define BT\_GATT\_PERM\_READ\_ENCRYPT  0x0004

> Attribute read permission with encryption.
> 
> If set, requires encryption for read access.

#### \#define BT\_GATT\_PERM\_WRITE\_ENCRYPT  0x0008

> Attribute write permission with encryption.
> 
> If set, requires encryption for write access.

#### \#define BT\_GATT\_PERM\_READ\_AUTHEN  0x0010

> Attribute read permission with authentication.
> 
> If set, requires encryption using authenticated link-key for read
> access.

#### \#define BT\_GATT\_PERM\_WRITE\_AUTHEN  0x0020

> Attribute write permission with authentication.
> 
> If set, requires encryption using authenticated link-key for write
> access.

#### \#define BT\_GATT\_PERM\_PREPARE\_WRITE  0x0040

> Attribute prepare write permission.
> 
> If set, allows prepare writes with use of
> BT\_GATT\_WRITE\_FLAG\_PREPARE passed to write callback.

#### \#define BT\_GATT\_PERM\_READ\_LESC  0x0080

> Attribute read permission with LE Secure Connection encryption.
> 
> If set, requires that LE Secure Connections is used for read access.

#### \#define BT\_GATT\_PERM\_WRITE\_LESC  0x0100

> Attribute write permission with LE Secure Connection encryption.
> 
> If set, requires that LE Secure Connections is used for write access.

#### \#define BT\_GATT\_CCC\_NOTIFY  0x0001

> Client Characteristic Configuration Values Bluetooth GATT Client
> Characteristic Configuration notification.
> 
> If set, changes to Characteristic Value are notified.
> 
> Bluetooth GATT Client Characteristic Configuration values Bluetooth
> GATT Client Characteristic Configuration notification
> 
> If set, changes to Characteristic Value are notified.

#### \#define BT\_GATT\_CCC\_INDICATE  0x0002

> Bluetooth GATT Client Characteristic Configuration indication.
> 
> If set, changes to Characteristic Value are indicated.
> 
> Bluetooth GATT Client Characteristic Configuration indication.
> 
> If set, changes to Characteristic Value are indicated.

#### \#define UUID\_GATT\_PRIMARY  0x2800

> GATT Primary Service UUID

#### \#define UUID\_GATT\_CCC  0x2902

> GATT Client Characteristic Configuration UUID

#### \#define BT\_GATT\_CCC\_NOTIFY  0x0001

> Client Characteristic Configuration Values Bluetooth GATT Client
> Characteristic Configuration notification.
> 
> If set, changes to Characteristic Value are notified.
> 
> Bluetooth GATT Client Characteristic Configuration values Bluetooth
> GATT Client Characteristic Configuration notification
> 
> If set, changes to Characteristic Value are notified.

#### \#define BT\_GATT\_CCC\_INDICATE  0x0002

> Bluetooth GATT Client Characteristic Configuration indication.
> 
> If set, changes to Characteristic Value are indicated.
> 
> Bluetooth GATT Client Characteristic Configuration indication.
> 
> If set, changes to Characteristic Value are indicated.

#### \#define BT\_DATA\_FLAGS  0x01

> EIR (Extended Inquiry Response)/AD (Advertising Data) data type
> definitions AD flags

#### \#define BT\_DATA\_UUID16\_SOME  0x02

> 16-bit UUID, more available

#### \#define BT\_DATA\_UUID16\_ALL  0x03

> 16-bit UUID, all listed

#### \#define BT\_DATA\_UUID32\_SOME  0x04

> 32-bit UUID, more available

#### \#define BT\_DATA\_UUID32\_ALL  0x05

> 32-bit UUID, all listed

#### \#define BT\_DATA\_UUID128\_SOME  0x06

> 128-bit UUID, more available

#### \#define BT\_DATA\_UUID128\_ALL  0x07

> 128-bit UUID, all listed

#### \#define BT\_DATA\_NAME\_SHORTENED  0x08

> Shortened name

#### \#define BT\_DATA\_NAME\_COMPLETE  0x09

> Complete name

#### \#define BT\_DATA\_TX\_POWER  0x0a

> Tx Power

#### \#define BT\_DATA\_SM\_TK\_VALUE  0x10

> Security Manager TK Value

#### \#define BT\_DATA\_SM\_OOB\_FLAGS  0x11

> Security Manager OOB Flags

#### \#define BT\_DATA\_SOLICIT16  0x14

> Solicit UUIDs, 16-bit

#### \#define BT\_DATA\_SOLICIT128  0x15

> Solicit UUIDs, 128-bit

#### \#define BT\_DATA\_SVC\_DATA16  0x16

> Service data, 16-bit UUID

#### \#define BT\_DATA\_GAP\_APPEARANCE  0x19

> GAP appearance

#### \#define BT\_DATA\_LE\_BT\_DEVICE\_ADDRESS  0x1b

> LE Bluetooth Device Address

#### \#define BT\_DATA\_LE\_ROLE  0x1c

> LE Role

#### \#define BT\_DATA\_SOLICIT32  0x1f

> Solicit UUIDs, 32-bit

#### \#define BT\_DATA\_SVC\_DATA32  0x20

> Service data, 32-bit UUID

#### \#define BT\_DATA\_SVC\_DATA128  0x21

> Service data, 128-bit UUID

#### \#define BT\_DATA\_LE\_SC\_CONFIRM\_VALUE  0x22

> LE Secure Connections Confirmation Value

#### \#define BT\_DATA\_LE\_SC\_RANDOM\_VALUE  0x23

> LE Secure Connections Random Value

#### \#define BT\_DATA\_URI  0x24

> Uniform Resource Identifier

#### \#define BT\_DATA\_LE\_SUPPORTED\_FEATURES  0x27

> LE Supported Features

#### \#define BT\_DATA\_CHANNEL\_MAP\_UPDATE\_IND  0x28

> Channel Map Update Indication

#### \#define BT\_DATA\_MESH\_PROV  0x29

> Mesh Provisioning PDU

#### \#define BT\_DATA\_MESH\_MESSAGE  0x2a

> Mesh Networking PDU

#### \#define BT\_DATA\_MESH\_BEACON  0x2b

> Mesh Beacon

#### \#define BT\_DATA\_BIG\_INFO  0x2c

> BIG Information

#### \#define BT\_DATA\_BROADCAST\_CODE  0x2d

> Broadcast Code

#### \#define BT\_DATA\_CSIS\_RSI  0x2e

> CSIS Random Set ID type

#### \#define CENTRAL\_HTC\_SERVICE\_ID  4

> Device central profile service ID

# Index

INDEX

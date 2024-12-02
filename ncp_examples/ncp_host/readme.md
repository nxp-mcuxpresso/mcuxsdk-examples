# ncp_host

## Overview
This is a ncp host demo. Need to use with ncp_device application.


## Running the demo
1.  Enter 'help' in the terminal to get help information.

    help
    wlan-scan
    get-scan-result
    wlan-connect <ssid>
    get-scan-result
    wlan-disconnect
    ping [c <packet_count>] <ip_address>
    get-ping-result
    iperf [s|su|c <host>|cu <host>|a|] [options]
    get-iperf-result

2.  Enter command

    #wlan-scan

    #get-scan-result

     3 networks found:
      94:10:3E:02:60:F0  [nxp_mrvl]
              channel: 1
              rssi: -25 dBm
              security: OPEN

      94:10:3E:02:60:F1  [nxp_mrvl_5ghz]
              channel: 36
              rssi: -39 dBm
              security: WPA/WPA2 Mixed

      90:72:40:21:B3:1A  [apple_g]
              channel: 11
              rssi: -51 dBm
              security: WPA3 SAE


    #wlan-connect net-5g

    #get-connect-result

     Already connected to an AP:
     SSID = {net-5g}
     IPv4 Address: {192.168.0.97}


    #wlan-disconnect

    Already disconnect to network

    #ping 192.168.0.1 c 20
 
    #get-ping-result

    ---  ping statistics  ---
    20 packets transmitted, 20 packets received, 0% packets loss

    #iperf c 192.168.0.1 t 20
    
    #get-iperf-result

    ---------------------
    TCP_DONE_CLIENT (TX)
    Local address : 192.168.0.97  Port : 49153
    Remote address : 192.168.0.128  Port : 5001
    Bytes Transferred 106642804
    Duration (ms) 20000
    BandWidth (Mbit/sec) 42

## Supported Boards
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/ncp_examples/ncp_host/example_board_readme.md)

Hardware requirements
=====================
- Mini/micro USB cable
- Target microcontroller board
- Personal Computer
- FreeMASTER 3.1.2 with TCP/UDP communication plug-in installed

Board settings
============
This demo is configured to communicate over a WiFi network. Wifi firmware must be already in flash otherwise write the firmware image to flash first. Make sure the network SSID and password are configured in freemaster_cfg.h or directly in network_wifi.c file. The SSID and password must match the local WiFi settings. This demo uses automatic DHCP address resolution so also make sure this service is enabled on the local network.

Prepare the demo
===============
1.  Interconnect the board with a switch and the host computer.
2.  Compile and download the program to the target microcontroller.
3.  Run and resume application execution when debugger stops in the main() function.

Connect with FreeMASTER
=======================
4.  Run FreeMASTER, use the Connection Wizard or open Project Options.
5.  Select network communication and configure the plug-in for TCP or UDP protocol with the port
that matches the settings in the freemaster_cfg.h.
6.  Start communication, FreeMASTER loads the initial TSA Active Content links in the Welcome page.
7.  Click the "FreeMASTER Demonstration Project (embedded in device)" in the Welcome page.
8.  The demo is now running, you should be able to watch variable values and graphs.

More information
================
Read more information about FreeMASTER tool at http://www.nxp.com/freemaster.
Feel free to ask questions and report issues at FreeMASTER's 
community page at https://community.nxp.com/community/freemaster

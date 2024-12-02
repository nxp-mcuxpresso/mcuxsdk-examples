# cadc_expansion_mux_aux_opamp_err051396

## Overview

The CADC expansion mux aux OPAMP errata err051396 application demonstrates when ADC mux aux circle may stop 
by failure, and how to apply the workaround to fix it. This application is based on CADC expansion mux aux OPAMP 
application, please refer to it for additional information.
The difference is that the OPAMPA/B output is connected to ADCA/B within chip without board external wire 
connections. The ADC mux aux scan mode is changed to scan mode 2: The sample completion of either ANA4/ANB4 or 
ADCA/ADCB on-chip analog signal enables subsequent selected channel.

In this application, the scan sequence is defined as:
[A7 A0 A0 B7 A0 A0 B7 A0 A0 A7 A0 A0 B7 A0 A0 A7]
After 3 rounds of scan, the ADC mux aux circle for ADCB stops, and will not continue. This makes the measurement 
on ADCB (value1 in promped message) not change again.
By applying workaround, the above error is fixed, and ADC mux aux circle for ADCB continues normally.
For more details on err051396, please check errata document.

## Supported Boards

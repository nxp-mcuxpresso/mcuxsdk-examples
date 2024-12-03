# cadc_lptmr_trigger

## Overview
This is an CADC demo application which shows how to use hardware trigger sources to handle
the CADC hardware trigger function. The Low Power Timer (LPTMR) is a period timer source and the CADC
hardware trigger event. Because the LPTMR trigger event can be used to trigger one of the CADC ,
this demo uses the LPTMR as a trigger source for a CADCx channel. The LPTMR triggers the CADC in a
fixed frequency and the demo gets the CADC conversion result in the CADC Conversion Complete (COCO) interrupt.
This demo uses DAC fifo to generate sinewave on DACx_OUT.

# dsc_pdb_adc16_trigger

## Overview

The pdb_adc16_trigger example shows how to use the PDB to generate a ADC trigger.

Based on the basic counter, to use the ADC trigger, just to enable the ADC trigger's "milestone" and set the user-
defined value for it.
After the PDB counter is triggered to start, when the counter pass the "milestone", the ADC's Pre-Trigger would be
generated and sent to the ADC module.

In this example, the ADC16 is configured with hardware trigger and conversion complete interrupt enabled.
Once it gets the trigger from the PDB, the conversion goes, then the ISR would be executed.

## Supported Boards
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/pdb/adc16_trigger/example_board_readme.md)

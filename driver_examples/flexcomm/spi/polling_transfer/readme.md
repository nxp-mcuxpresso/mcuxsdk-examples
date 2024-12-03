# spi_polling_transfer

## Overview
The spi_polling example shows how to use spi driver with polling:

In this example, one spi instance as master and another spi instance as slave. Master sends a piece of data to slave,
and check if the data slave received is correct.

Notice: The SPI slave of this example uses interrupt mode, as there is no polling mode for SPI slave.

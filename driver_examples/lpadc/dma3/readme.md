# LPADC DMA3 Example

The LPADC DMA3 example shows how to use the LPADC FIFO watermark DMA request
with the DMA3 driver.

In this example, typing any key into the debug console triggers an LPADC
software conversion sequence. The LPADC stores conversion results in the result
FIFO. Each FIFO watermark request triggers DMA3 to move one result word from the
LPADC result FIFO to memory. After the DMA transfer completes, the application
prints the sampled conversion value.

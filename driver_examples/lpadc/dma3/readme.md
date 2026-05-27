# LPADC DMA3 Example

The LPADC DMA3 example shows how to use the LPADC FIFO watermark DMA request
with the DMA3 driver.

In this example, typing any key into the debug console triggers an LPADC
software conversion sequence. The LPADC stores conversion results in the result
FIFO. Each FIFO watermark request triggers DMA3 to move one result word from the
LPADC result FIFO to memory. After the DMA transfer completes, the application
prints the sampled conversion value.

## Supported Boards
- [FRDM-KW43](../../../_boards/frdmkw43/driver_examples/lpadc/dma3/example_board_readme.md)
- [FRDM-MCXW70](../../../_boards/frdmmcxw70/driver_examples/lpadc/dma3/example_board_readme.md)


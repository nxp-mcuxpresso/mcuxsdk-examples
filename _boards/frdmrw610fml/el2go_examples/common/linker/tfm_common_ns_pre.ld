;
MEMORY
{
  FLASH (rx) : ORIGIN = (((0x08000000) + ((((0x0) + (0x1000) + (0xA0000 - (0x1000))) + (0x0))))), LENGTH = (((0x300000) - (0x0) - (0x0)))
  RAM (rwx) : ORIGIN = (((0x20000000) + (((0x10000)) + ((((0x130000)) - ((0x10000)) ) / 6)))), LENGTH = (((0x130000)) - ((((0x130000)) - ((0x10000)) ) / 6) - ((0x10000)))
  m_mbox1 (RW) : ORIGIN = 0x41380000, LENGTH = 0x00000488
  m_txq1 (RW) : ORIGIN = 0x41380488, LENGTH = 0x00001000
  m_mbox2 (RW) : ORIGIN = 0x443C0000, LENGTH = 0x00000488
  m_txq23 (RW) : ORIGIN = 0x443C0488, LENGTH = 0x00001080
  m_txq32 (RW) : ORIGIN = 0x443C1508, LENGTH = 0x00001080
}
__heap_size__ = (0x0004000);
__stack_size__ = (0x0002000);
GROUP(libgcc.a libc.a libm.a libnosys.a)
ENTRY(Reset_Handler)
SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;
        __end__ = .;
        *(.text*)
        KEEP(*(.init))
        KEEP(*(.fini))
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)
        *(.rodata*)
        KEEP(*(.eh_frame*))
    } > FLASH
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;
    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG ((__data_end__ - __data_start__) / 4)
        LONG (DEFINED(__etext2) ? __etext2 : 0)
        LONG (DEFINED(__data2_start__) ? __data2_start__ : 0)
        LONG (DEFINED(__data2_start__) ? ((__data2_end__ - __data2_start__) / 4) : 0)
        __copy_table_end__ = .;
    } > FLASH
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG ((__bss_end__ - __bss_start__) / 4)
        LONG (DEFINED(__bss2_start__) ? __bss2_start__ : 0)
        LONG (DEFINED(__bss2_start__) ? ((__bss2_end__ - __bss2_start__) / 4) : 0)
        __zero_table_end__ = .;
    } > FLASH
    __etext = ALIGN(4);
    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        KEEP(*(.jcr*))
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM
    bss_size = __bss_end__ - __bss_start__;
    .stack : ALIGN(32)
    {
        . += __stack_size__;
    } > RAM
    __StackLimit = ADDR(.stack);
    __StackTop = ADDR(.stack) + SIZEOF(.stack);
    .heap : ALIGN(8)
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .;
    } > RAM
    PROVIDE(__stack = __StackTop);
    .smu_cpu13_mbox (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu13_mbox)
        KEEP (*(.smu_cpu13_mbox))
        . = ALIGN(4);
    } > m_mbox1
    .smu_cpu31_txq (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu31_txq)
        KEEP (*(.smu_cpu31_txq))
        . = ALIGN(4);
    } > m_txq1
    .smu_cpu23_mbox (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu23_mbox)
        KEEP (*(.smu_cpu23_mbox))
        . = ALIGN(4);
    } > m_mbox2
    .smu_cpu32_txq (NOLOAD) :
    {
        . = ALIGN(4);
        *(.smu_cpu32_txq)
        KEEP (*(.smu_cpu32_txq))
        . = ALIGN(4);
    } > m_txq32
}

M_CRASH_DATA_RAM_SIZE = 0x100;
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 0x100000 - 128k
  DATA  (rwx) : ORIGIN = 0x080E0000, LENGTH = 128k
  SRAM2 (rwx) : ORIGIN = 0x10000000 + (((98 * 4) + 7) & 0xFFFFFFF8), LENGTH = 0x8000 - (((98 * 4) + 7) & 0xFFFFFFF8)
  SRAM1 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x18000
}
ENTRY(Reset_Handler)
SECTIONS
{
    .user_data :
    {
        . = ALIGN(8);
        *(.user_data)
        . = ALIGN(8);
    } > DATA
    .text :
    {
        KEEP(*(.isr_vector))
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
    __etext = .;
    _sidata = .;
    .crash_data_ram :
    {
        . = ALIGN(8);
        __CRASH_DATA_RAM__ = .;
        __CRASH_DATA_RAM_START__ = .;
        KEEP(*(.keep.crash_data_ram))
        *(.m_crash_data_ram)
        . += M_CRASH_DATA_RAM_SIZE;
        . = ALIGN(8);
        __CRASH_DATA_RAM_END__ = .;
    } > SRAM2
    .stack (NOLOAD):
    {
        __StackLimit = .;
        *(.stack*);
        . += 0x400 - (. - __StackLimit);
    } > SRAM2
    __StackTop = ADDR(.stack) + SIZEOF(.stack);
    _estack = __StackTop;
    __StackLimit = ADDR(.stack);
    PROVIDE(__stack = __StackTop);
    .heap_0 (COPY):
    {
        __mbed_sbrk_start_0 = .;
        . += (ORIGIN(SRAM2) + LENGTH(SRAM2) - .);
        __mbed_krbs_start_0 = .;
    } > SRAM2
    ASSERT(__mbed_krbs_start_0 <= (ORIGIN(SRAM2)+LENGTH(SRAM2)), "Heap is too big for SRAM2")
    .data : AT (__etext)
    {
        __data_start__ = .;
        _sdata = .;
        *(vtable)
        *(.data*)
        . = ALIGN(8);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(8);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(8);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        KEEP(*(.jcr*))
        . = ALIGN(8);
        __data_end__ = .;
        _edata = .;
    } > SRAM1
    ASSERT(__data_end__ <= (ORIGIN(SRAM1)+LENGTH(SRAM1)), ".data is too big for SRAM1")
    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
        _ebss = .;
    } > SRAM1
    ASSERT(__bss_end__ <= (ORIGIN(SRAM1)+LENGTH(SRAM1)), "BSS is too big for SRAM1")
    .heap (COPY):
    {
        __end__ = .;
        end = __end__;
        __mbed_sbrk_start = .;
        *(.heap*)
        . += (ORIGIN(SRAM1) + LENGTH(SRAM1) - .);
        __mbed_krbs_start = .;
        __HeapLimit = .;
    } > SRAM1
    ASSERT(__HeapLimit <= (ORIGIN(SRAM1)+LENGTH(SRAM1)), "Heap is too big for SRAM1")
}

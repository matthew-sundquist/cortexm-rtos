/* linker.ld - STM32L476RG minimal freestanding linker script */

ENTRY(Reset_Handler)

/* Memory layout */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Stack top */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Minimal heap/stack sizes */
_Min_Heap_Size  = 0x200;
_Min_Stack_Size = 0x800;

SECTIONS
{
    /* Vector table + code */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
    } > FLASH

    /* ARM exception unwind (needed by GCC) */
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH

    /* Initialized data */
    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM

    _sidata = LOADADDR(.data);

    /* Uninitialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* Heap start (optional but useful for _sbrk) */
    _end = .;
}

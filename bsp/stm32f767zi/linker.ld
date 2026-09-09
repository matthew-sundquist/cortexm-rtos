/* https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Projects/STM32F767ZI-Nucleo/Examples/FLASH/FLASH_EraseProgram/SW4STM32/STM32F767ZI-Nucleo/STM32F767ZITx_FLASH.ld */
/* ^ example linker script for stm32f767zi */
ENTRY(Reset_Handler)

/* Memory layout */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 512K
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

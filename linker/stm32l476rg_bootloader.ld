ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    . = ORIGIN(FLASH);

    .isr_vector ALIGN(4) :
    {
        __isr_vector_start = .;

        KEEP(*(.isr_vector))
        KEEP(*(.isr_vector.*))

        __isr_vector_end = .;
    } > FLASH

    .text ALIGN(4) :
    {
        *(.text)
        *(.text.*)

        . = ALIGN(4);
    } > FLASH

    .rodata ALIGN(4) :
    {
        *(.rodata)
        *(.rodata.*)

        . = ALIGN(4);
    } > FLASH

    .preinit_array ALIGN(4) :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);

        KEEP(*(.preinit_array))
        KEEP(*(.preinit_array.*))

        PROVIDE_HIDDEN(__preinit_array_end = .);

        . = ALIGN(4);
    } > FLASH

    .init_array ALIGN(4) :
    {
        PROVIDE_HIDDEN(__init_array_start = .);

        KEEP(*(.init_array))
        KEEP(*(.init_array.*))

        PROVIDE_HIDDEN(__init_array_end = .);

        . = ALIGN(4);
    } > FLASH

    .fini_array ALIGN(4) :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);

        KEEP(*(.fini_array))
        KEEP(*(.fini_array.*))

        PROVIDE_HIDDEN(__fini_array_end = .);

        . = ALIGN(4);
    } > FLASH

    _sidata = LOADADDR(.data);

    .data ALIGN(4) :
    {
        _sdata = .;

        *(.data)
        *(.data.*)

        . = ALIGN(4);

        _edata = .;
    } > SRAM AT > FLASH

    .bss ALIGN(4) :
    {
        _sbss = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
    } > SRAM

    .noinit ALIGN(4) :
    {
        _snoinit = .;

        *(.noinit)
        *(.noinit.*)

        . = ALIGN(4);

        _enoinit = .;
    } > SRAM

    _estack = ORIGIN(SRAM) + LENGTH(SRAM);

    _end = .;

    ASSERT(
        _end <= ORIGIN(SRAM) + LENGTH(SRAM),
        "ERROR: SRAM overflow"
    )

    ASSERT(
        __isr_vector_start == ORIGIN(FLASH),
        "ERROR: Vector table is not at Flash origin"
    )

    ASSERT(
        __isr_vector_end <= ORIGIN(FLASH) + LENGTH(FLASH),
        "ERROR: Bootloader Flash overflow"
    )

    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
        *(.ARM.attributes)
        *(.comment)
        *(.debug_frame)
    }
}
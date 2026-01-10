ENTRY(main)

MEMORY
{
    RAM (xrw) : ORIGIN = 0x08160000 , LENGTH = 64K
}

SECTIONS
{
    .text :
    {
        *(.text)
        *(.text.*)
        . = ALIGN(4);
    } >RAM

    .data :
    {
        *(.data)
        . = ALIGN(4);
    } >RAM

    .bss :
    {
        *(.bss)
        . = ALIGN(4);
    } >RAM
}
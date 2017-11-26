ENTRY(_start);
SECTIONS {
    . = 0x30000000;
    
    .text          :   
    {
    ./start.o (.text)
    *(.text) 
    }
    .rodata ALIGN(4) : {*(.rodata)} 
    .data ALIGN(4) : { *(.data) }
    .bss ALIGN(4)  : { *(.bss)  *(COMMON) }
}


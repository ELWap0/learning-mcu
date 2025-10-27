ENTRY(Reset_Handler)

MEMORY {
  flash (rx)  : ORIGIN = 0x08000000, LENGTH = 256k
  sram  (rwx) : ORIGIN = 0x20000000, LENGTH = 64k
}

_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
  .isr_vector : {
    KEEP(*(.isr_vector))
  } >flash

  .text : {
    _stext = .;
    *(.text*)
    _etext = .;
  } >flash

  .rodata : {
    *(.rodata*)
  } >flash

  .data : {
    _sdata = .;
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;
    _sidata = LOADADDR(.data);
  } >sram AT>flash

  .bss : {
    _sbss = .;
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;
  } >sram

  . = ALIGN(8);
  _end = .;
}


/* SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna */
/* SPDX-License-Identifier: Apache-2.0 */

INCLUDE common.ldh

SECTIONS {
  .text : {
    *(.text._start)
    *(.text)
    *(.text.*)
  } > memisl

  .misc : ALIGN(16) {
    *(.rodata)
    *(.rodata.*)
    *(.data)
    *(.data.*)
    *(.srodata)
    *(.srodata.*)
    *(.sdata)
    *(.sdata.*)
  } > memisl

  . = ALIGN(32);
  __bss_start = .;
  .bss : {
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
  } > memisl
  . = ALIGN(32);
  __bss_end = .;

  .bulk : ALIGN(16) {
    *(.bulk)
    *(.bulk.*)
  } > memisl

  .l2_heap : ALIGN(16) {
    __l2_heap_start = .;
    __l2_heap_end = ORIGIN(memisl) + LENGTH(memisl);
  } > memisl

  .dram :
  {
    __dram_start = .;
    *(.dram)
    __dram_end = .;
  } > dram

  .l3_heap : ALIGN(16) {
    __l3_heap_start = .;
    __l3_heap_end = ORIGIN(dram) + LENGTH(dram);
  } > dram
}

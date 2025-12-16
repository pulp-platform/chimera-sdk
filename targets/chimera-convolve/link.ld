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

  .bss (NOLOAD) : {
    . = ALIGN(32);
    __bss_start = .;
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
    . = ALIGN(32);
    __bss_end = .;
  } > memisl

  /* Thread Local Storage sections */
  .tdata :
  {
    . = ALIGN(32);
    __tdata_start = .;
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    . = ALIGN(32);
    __tdata_end = .;
  } > memisl

  .tbss (NOLOAD) :
  {
    . = ALIGN(32);
    __tbss_start = .;
    *(.tbss .tbss.* .gnu.linkonce.tb.*)
    *(.tcommon)
    . = ALIGN(32);
    __tbss_end = .;
  } > memisl

  /* Cluster Local Storage sections */
  .cdata :
  {
    . = ALIGN(32);
    __cdata_start = .;
    *(.cdata .cdata.*)
    . = ALIGN(32);
    __cdata_end = .;
  } > l1_alias AT > memisl
  /* Provide symbols for the load address (LMA) of the .cdata section so
     runtime code can copy the initialized contents from flash/main memory
     instead of using the VMA which points into L1. */
  __cdata_lma_start = LOADADDR(.cdata);
  __cdata_lma_end = LOADADDR(.cdata) + SIZEOF(.cdata);

  .cbss (NOLOAD) :
  {
    . = ALIGN(32);
    __cbss_start = .;
    *(.cbss .cbss.*)
    . = ALIGN(32);
    __cbss_end = .;
  } > l1_alias AT > memisl

  __cbss_lma_start = LOADADDR(.cbss);
  __cbss_lma_end = LOADADDR(.cbss) + SIZEOF(.cbss);

  /* Reserve the rest of cluster-local L1 for general allocations. */
  __cls_reserved_size = SIZEOF(.cdata) + SIZEOF(.cbss);

  .bulk : ALIGN(16) {
    *(.bulk)
    *(.bulk.*)
  } > memisl

  .l1_c0_cls  (NOLOAD): ALIGN(16) {
    . += __cls_reserved_size;
  } > l1_c0

  .l1_c1_cls  (NOLOAD): ALIGN(16) {
    . += __cls_reserved_size;
  } > l1_c1

  .l1_c2_cls  (NOLOAD): ALIGN(16) {
    . += __cls_reserved_size;
  } > l1_c2
  .l1_c3_cls  (NOLOAD): ALIGN(16) {
    . += __cls_reserved_size;
  } > l1_c3
  .l1_c4_cls  (NOLOAD): ALIGN(16) {
    . += __cls_reserved_size;
  } > l1_c4

  .l1_c0 : ALIGN(16) {
    __l1_c0_start = .;
    *(.l1_c0 .l1_c0.*)
    __l1_c0_end = .;
    . = ALIGN(16);
    __l1_c0_heap_start = .;
    __l1_c0_heap_end = ORIGIN(l1_c0) + LENGTH(l1_c0);
  } > l1_c0 AT > memisl

  /* Provide symbols for the load address (LMA) of the .l1_c0 section so
     runtime code can copy the initialized contents from flash/main memory
     instead of using the VMA which points into L1.
     WIESEP: This is also required as JTAG can somehow not access L1 memory...*/
  __l1_c0_lma_start = LOADADDR(.l1_c0);
  __l1_c0_lma_end = LOADADDR(.l1_c0) + SIZEOF(.l1_c0);

  .l1_c1 : ALIGN(16) {
    __l1_c1_start = .;
    *(.l1_c1 .l1_c1.*)
    __l1_c1_end = .;
    . = ALIGN(16);
    __l1_c1_heap_start = .;
    __l1_c1_heap_end = ORIGIN(l1_c1) + LENGTH(l1_c1);
  } > l1_c1 AT > memisl

  __l1_c1_lma_start = LOADADDR(.l1_c1);
  __l1_c1_lma_end = LOADADDR(.l1_c1) + SIZEOF(.l1_c1);


  .l1_c2 : ALIGN(16) {
    __l1_c2_start = .;
    *(.l1_c2 .l1_c2.*)
    __l1_c2_end = .;
    . = ALIGN(16);
    __l1_c2_heap_start = .;
    __l1_c2_heap_end = ORIGIN(l1_c2) + LENGTH(l1_c2);
  } > l1_c2 AT > memisl

  __l1_c2_lma_start = LOADADDR(.l1_c2);
  __l1_c2_lma_end = LOADADDR(.l1_c2) + SIZEOF(.l1_c2);

  .l1_c3 : ALIGN(16) {
    __l1_c3_start = .;
    *(.l1_c3 .l1_c3.*)
    __l1_c3_end = .;
    . = ALIGN(16);
    __l1_c3_heap_start = .;
    __l1_c3_heap_end = ORIGIN(l1_c3) + LENGTH(l1_c3);
  } > l1_c3 AT > memisl

  __l1_c3_lma_start = LOADADDR(.l1_c3);
  __l1_c3_lma_end = LOADADDR(.l1_c3) + SIZEOF(.l1_c3);

  .l1_c4 : ALIGN(16) {
    __l1_c4_start = .;
    *(.l1_c4 .l1_c4.*)
    __l1_c4_end = .;
    . = ALIGN(16);
    __l1_c4_heap_start = .;
    __l1_c4_heap_end = ORIGIN(l1_c4) + LENGTH(l1_c4);
  } > l1_c4 AT > memisl

  __l1_c4_lma_start = LOADADDR(.l1_c4);
  __l1_c4_lma_end = LOADADDR(.l1_c4) + SIZEOF(.l1_c4);

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

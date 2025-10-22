/* Copyright 2022 ETH Zurich and University of Bologna. */
/* Licensed under the Apache License, Version 2.0, see LICENSE for details. */
/* SPDX-License-Identifier: Apache-2.0 */

/* Nicole Narr <narrn@student.ethz.ch> */
/* Christopher Reinwardt <creinwar@student.ethz.ch> */
/* Paul Scheffler <paulsc@iis.ee.ethz.ch> */
/* Lorenzo Leone <lleone@iis.ee.ethz.ch> */


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
  .tdata    :
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
  .cdata    :
  {
    . = ALIGN(32);
    __cdata_start = .;
    *(.cdata .cdata.*)
    . = ALIGN(32);
    __cdata_end = .;
  } > l1_c4 AT > memisl
  /* Provide symbols for the load address (LMA) of the .cdata section so
     runtime code can copy the initialized contents from flash/main memory
     instead of using the VMA which points into L1. */
  __cdata_lma_start = LOADADDR(.cdata);
  __cdata_lma_end = LOADADDR(.cdata) + SIZEOF(.cdata);

  .cbss :
  {
    . = ALIGN(32);
    __cbss_start = .;
    *(.cbss .cbss.*)
    . = ALIGN(32);
    __cbss_end = .;
  } > l1_c4 AT > memisl


  .heap : ALIGN(16) {
    __heap_start = .;
    __heap_end = ORIGIN(memisl) + LENGTH(memisl);
  } > memisl

  .bulk : ALIGN(16) {
    *(.bulk)
    *(.bulk.*)
  } > memisl

  .l1 (NOLOAD) : ALIGN(16) {
    *(.l1 .l1.*)
  } > l1

  .l1_c0 : ALIGN(16) {
    __l1_c0_start = .;
    *(.l1_c0 .l1_c0.*)
    __l1_c0_end = .;
  } > l1_c0

  .l1_c1 : ALIGN(16) {
    __l1_c1_start = .;
    *(.l1_c1 .l1_c1.*)
    __l1_c1_end = .;
  } > l1_c1

  .l1_c2 : ALIGN(16) {
    __l1_c2_start = .;
    *(.l1_c2 .l1_c2.*)
    __l1_c2_end = .;
  } > l1_c2

  .l1_c3 : ALIGN(16) {
    __l1_c3_start = .;
    *(.l1_c3 .l1_c3.*)
    __l1_c3_end = .;
  } > l1_c3

  .l1_c4 : ALIGN(16) {
    __l1_c4_start = .;
    *(.l1_c4 .l1_c4.*)
    __l1_c4_end = .;
  } > l1_c4

  .dram :
  {
    *(.dram)
    _edram = .;
  } > dram
}

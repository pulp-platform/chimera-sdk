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

  /* NOTE: .cbss is NOLOAD, so no bytes are written to the LMA by the loader.
   * Zero-init is performed via the hardware zero-memory region
   * (see snrt_zero_memory_ptr()). */
  __cbss_lma_start = LOADADDR(.cbss);
  __cbss_lma_end = LOADADDR(.cbss) + SIZEOF(.cbss);

  /* Total cluster-local storage (CLS) reserved at the base of every cluster's
   * L1 TCDM.  Must not exceed the l1_alias window (= L1 size per cluster). */
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

  /* Provide LMA symbols for the .l1_cX sections so the runtime DMA init code
   * can copy cluster-private initialized data from memisl (LMA) to each
   * cluster's L1 TCDM (VMA).  Two reasons the LMA copy is necessary:
   *  1. The binary loader places data at the LMA (memisl); the VMA points
   *     into cluster L1 which is not reachable by the loader.
   *  2. Some debug probes (JTAG) cannot reliably access cluster L1 during
   *     early boot, so the DMA copy from memisl is the only safe path.
   *
   * LMA symbols are assigned inside the section body so that lld only
   * evaluates LOADADDR/SIZEOF when the section actually has input sections.
   * PROVIDE(...= 0) outside gives a safe fallback: the runtime sees
   * lma_end - lma_start = 0 and skips the DMA for empty sections. */
  .l1_c0 : ALIGN(16) {
    __l1_c0_start = .;
    __l1_c0_lma_start = LOADADDR(.l1_c0);
    KEEP(*(.l1_c0 .l1_c0.*))
    __l1_c0_end = .;
    __l1_c0_lma_end = LOADADDR(.l1_c0) + SIZEOF(.l1_c0);
    . = ALIGN(16);
    __l1_c0_heap_start = .;
    __l1_c0_heap_end = ORIGIN(l1_c0) + LENGTH(l1_c0);
  } > l1_c0 AT > memisl
  PROVIDE(__l1_c0_lma_start = 0);
  PROVIDE(__l1_c0_lma_end = 0);

  .l1_c1 : ALIGN(16) {
    __l1_c1_start = .;
    __l1_c1_lma_start = LOADADDR(.l1_c1);
    KEEP(*(.l1_c1 .l1_c1.*))
    __l1_c1_end = .;
    __l1_c1_lma_end = LOADADDR(.l1_c1) + SIZEOF(.l1_c1);
    . = ALIGN(16);
    __l1_c1_heap_start = .;
    __l1_c1_heap_end = ORIGIN(l1_c1) + LENGTH(l1_c1);
  } > l1_c1 AT > memisl
  PROVIDE(__l1_c1_lma_start = 0);
  PROVIDE(__l1_c1_lma_end = 0);

  .l1_c2 : ALIGN(16) {
    __l1_c2_start = .;
    __l1_c2_lma_start = LOADADDR(.l1_c2);
    KEEP(*(.l1_c2 .l1_c2.*))
    __l1_c2_end = .;
    __l1_c2_lma_end = LOADADDR(.l1_c2) + SIZEOF(.l1_c2);
    . = ALIGN(16);
    __l1_c2_heap_start = .;
    __l1_c2_heap_end = ORIGIN(l1_c2) + LENGTH(l1_c2);
  } > l1_c2 AT > memisl
  PROVIDE(__l1_c2_lma_start = 0);
  PROVIDE(__l1_c2_lma_end = 0);

  .l1_c3 : ALIGN(16) {
    __l1_c3_start = .;
    __l1_c3_lma_start = LOADADDR(.l1_c3);
    KEEP(*(.l1_c3 .l1_c3.*))
    __l1_c3_end = .;
    __l1_c3_lma_end = LOADADDR(.l1_c3) + SIZEOF(.l1_c3);
    . = ALIGN(16);
    __l1_c3_heap_start = .;
    __l1_c3_heap_end = ORIGIN(l1_c3) + LENGTH(l1_c3);
  } > l1_c3 AT > memisl
  PROVIDE(__l1_c3_lma_start = 0);
  PROVIDE(__l1_c3_lma_end = 0);

  .l1_c4 : ALIGN(16) {
    __l1_c4_start = .;
    __l1_c4_lma_start = LOADADDR(.l1_c4);
    KEEP(*(.l1_c4 .l1_c4.*))
    __l1_c4_end = .;
    __l1_c4_lma_end = LOADADDR(.l1_c4) + SIZEOF(.l1_c4);
    . = ALIGN(16);
    __l1_c4_heap_start = .;
    __l1_c4_heap_end = ORIGIN(l1_c4) + LENGTH(l1_c4);
  } > l1_c4 AT > memisl
  PROVIDE(__l1_c4_lma_start = 0);
  PROVIDE(__l1_c4_lma_end = 0);

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

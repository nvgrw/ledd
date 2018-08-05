#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * The primary purpose of ledd is to spool LED commands.
 * As such, the command interface is quite primitive.
 *
 * Red: 8 bits
 * Grn: 8 bits
 * Blu: 8 bits
 * Wht: 8 bits
 * RnF: 8 bits
 * RnT: 8 bits
 * Ctr: [
 *        Hold: 1 bit
 *        Cont: 1 bit
 *      ]
 */

#define LEDD_INSTR_FLAG_HOLD 0x80
#define LEDD_INSTR_FLAG_CONT 0x40

typedef struct __attribute__((packed)) {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
  uint8_t range_from;
  uint8_t range_to;
  // FLAGS = 0x40 CONT 0x80
  uint8_t flags;
} ledd_instr_t;

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * An implementation of http://openpixelcontrol.org
 */

typedef enum {
  OPC_SET_8_BIT = 0,
  OPC_SET_16_BIT = 2,
  OPC_SYSTEM_EXCLUSIVE = 255
} opc_cmd_t;

typedef struct __attribute__((packed)) {
  uint8_t channel;
  opc_cmd_t command : 8;
  uint8_t length_high;
  uint8_t length_low;
} opc_header_t;

typedef struct __attribute__((packed)) {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} opc_pixel8_t;

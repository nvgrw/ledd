#pragma once

#include "rpi_ws281x/ws2811.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define NUM_LEDS 150
#define WRAPAROUND 84
#define TARGET_FREQ WS2811_TARGET_FREQ
#define GPIO_PIN 18
#define STRIP_TYPE SK6812_STRIP_GRBW  // Figure out how this is to be set

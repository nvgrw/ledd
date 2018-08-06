#include "ledd.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>

#include "rpi_ws281x/ws2811.h"

#include "config.h"
#include "instruction.h"

static const size_t INSTR_SIZE = sizeof(ledd_instr_t);

static void ledd_execute(const ledd_instr_t *instr);
static inline void ledd_set(uint8_t led_num, ws2811_led_t color);

/* 0xWWRRGGBB */
static inline ws2811_led_t ledd_color(uint8_t red, uint8_t green, uint8_t blue,
                                      uint8_t white);

static ws2811_t strip = {.freq = TARGET_FREQ,
                         .dmanum = 10,
                         .channel = {
                             [0] =
                                 {
                                     .gpionum = GPIO_PIN,
                                     .count = NUM_LEDS,
                                     .invert = 0,
                                     .brightness = 255,
                                     .strip_type = STRIP_TYPE,
                                 },
                             [1] =
                                 {
                                     .gpionum = 0,
                                     .count = 0,
                                     .invert = 0,
                                     .brightness = 0,
                                 },
                         }};

error_t ledd_run(void) {
  printf("ledd initializing\n");
  ws2811_return_t ws_ret;
  if ((ws_ret = ws2811_init(&strip)) != WS2811_SUCCESS) {
    return (error_t){.code = ERROR_CODE_STRIP_INIT_FAIL,
                     .message = ws2811_get_return_t_str(ws_ret)};
  }
  printf("ledd initialized\n");

  int amount_read = 0;
  ledd_instr_t instr;

  while ((amount_read = read(STDIN_FILENO, &instr, INSTR_SIZE)) > 0) {
    if (amount_read != INSTR_SIZE) {
      fprintf(stderr, "malformed instruction\n");
      continue;
    }

    ledd_execute(&instr);
  }

  return (error_t){.code = ERROR_CODE_SUCCESS, .message = NULL};
}

void ledd_shutdown(void) {
  printf("ledd shutting down\n");
  ws2811_fini(&strip);
  printf("ledd shut down\n");
}

static void ledd_execute(const ledd_instr_t *instr) {
  /* If the pattern is to be held, don't do anything */
  if (instr->flags & LEDD_INSTR_FLAG_HOLD) return;

  /* Determine range */
  uint8_t rng_fro = instr->range_from;
  uint8_t rng_to = MIN(instr->range_to, WRAPAROUND);
  if (rng_fro > rng_to) {
    rng_fro = rng_to;
  }

  const ws2811_led_t color =
      ledd_color(instr->red, instr->green, instr->blue, instr->white);
  for (int i = rng_fro; i < rng_to; i++) {
    ledd_set(i, color);
  }

  /* Continue assembling the pattern */
  if (instr->flags & LEDD_INSTR_FLAG_CONT) return;

  ws2811_return_t ws_ret;
  if ((ws_ret = ws2811_render(&strip)) != WS2811_SUCCESS) {
    fprintf(stderr, "Failed to render %s\n", ws2811_get_return_t_str(ws_ret));
  }
}

/**
 * Set `led_num` and its mapped counterpart to the desired color.
 * Currently only a single wraparound is supported.
 */
static inline void ledd_set(uint8_t led_num, ws2811_led_t color) {
  if (led_num >= WRAPAROUND) return;

  strip.channel[0].leds[led_num] = color;

  const uint8_t counterpart_num = 2 * WRAPAROUND - (led_num % WRAPAROUND);

  /* Validate counterpart and then either set both or only one */
  if (counterpart_num < NUM_LEDS)
    strip.channel[0].leds[counterpart_num] = color;
}

static inline ws2811_led_t ledd_color(uint8_t red, uint8_t green, uint8_t blue,
                                      uint8_t white) {
  // 0xWWRRGGBB
  return (((ws2811_led_t)red) << 16) | (((ws2811_led_t)green) << 8) |
         ((ws2811_led_t)blue) | (((ws2811_led_t)white) << 24);
}

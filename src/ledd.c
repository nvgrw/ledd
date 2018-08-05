#include "ledd.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "config.h"
#include "instruction.h"

static const size_t INSTR_SIZE = sizeof(ledd_instr_t);

static inline void ledd_execute(const ledd_instr_t *instr);
static inline void ledd_set(uint8_t led_num, uint8_t, uint8_t, uint8_t,
                            uint8_t);

error_t ledd_run(void) {
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

static inline void ledd_execute(const ledd_instr_t *instr) {
  /* Interact with API not present on macOS */
  printf("Executing: R%d G%d B%d W%d From%d To%d HOLD?%s CONT?%s\n", instr->red,
         instr->green, instr->blue, instr->white, instr->range_from,
         instr->range_to, (instr->flags & LEDD_INSTR_FLAG_HOLD) ? "tru" : "fls",
         (instr->flags & LEDD_INSTR_FLAG_CONT) ? "tru" : "fls");

  ledd_set(0, 0, 0, 0, 0);
}

/**
 * Set `led_num` and its mapped counterpart to the desired color.
 * Currently only a single wraparound is supported.
 */
static inline void ledd_set(uint8_t led_num, uint8_t red, uint8_t green,
                            uint8_t blue, uint8_t white) {
  /* Interact with API not present on macOS */
  /*
   * ----------->
   *            v
   * -----------<
   */

  // uint8_t counterpart_num = 2 * WRAPAROUND - (led_num % WRAPAROUND);

  /* Validate counterpart and then either set both or only one */
}

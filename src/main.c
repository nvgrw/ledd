#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "ledd.h"

void sigint_handler(int sig) {
  ledd_shutdown();
  exit(0);
}

int main(int argc, char **argv) {
  signal(SIGINT, sigint_handler);

  error_t error;
  if ((error = ledd_run()).code != ERROR_CODE_SUCCESS) {
    fprintf(stderr, "Fatal Error: %s, code: %d.\n", error.message, error.code);

    return EXIT_FAILURE;
  }

  ledd_shutdown();

  return EXIT_SUCCESS;
}

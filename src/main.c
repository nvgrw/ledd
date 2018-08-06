#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "ledd.h"

static void print_help(void) { fprintf(stderr, "ledd [-d]"); }

void sigint_handler(int sig) {
  ledd_shutdown();
  exit(0);
}

int main(int argc, char **argv) {
  bool daemonize = false;

  /* Should this process run in the background? */
  if (argc > 1) {
    if (strcmp("-d", argv[1]))
      daemonize = true;
    else
      print_help();
  }

  if (daemonize) {
  }

  signal(SIGINT, sigint_handler);

  /* TODO: Implement daemonization logic */

  error_t error;
  if ((error = ledd_run()).code != ERROR_CODE_SUCCESS) {
    fprintf(stderr, "Fatal Error: %s, code: %d.\n", error.message, error.code);

    return EXIT_FAILURE;
  }

  ledd_shutdown();

  return EXIT_SUCCESS;
}

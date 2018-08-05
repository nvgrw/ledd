#pragma once

enum error_code { ERROR_CODE_SUCCESS = 0, ERROR_CODE_NO_FIFO };

typedef struct {
  enum error_code code;
  const char *message;
} error_t;

#pragma once

enum error_code {
  ERROR_CODE_SUCCESS = 0,
  ERROR_CODE_STRIP_INIT_FAIL,
  ERROR_CODE_SOCKET_FAIL,
  ERROR_CODE_BIND_FAIL,
  ERROR_CODE_LISTEN_FAIL,
};

typedef struct {
  enum error_code code;
  const char *message;
} error_t;

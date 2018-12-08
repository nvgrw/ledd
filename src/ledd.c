#include "ledd.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#include "rpi_ws281x/ws2811.h"

#include "config.h"
#include "instruction.h"

#define PORT_NO 7890
#define MAX_PIXELS 300

static void ledd_execute(opc_pixel8_t* pixels, int num_pixels);
static inline void ledd_set(uint8_t led_num, ws2811_led_t color);

/* 0xWWRRGGBB */
static inline ws2811_led_t ledd_color(uint8_t red, uint8_t green, uint8_t blue,
                                      uint8_t white);
static inline ws2811_led_t ledd_color_no_white(uint8_t red, uint8_t green, uint8_t blue);

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

static int serverfd;

error_t ledd_run(void) {
  printf("ledd initializing\n");

  // Init ledd
  ws2811_return_t ws_ret;
  if ((ws_ret = ws2811_init(&strip)) != WS2811_SUCCESS) {
    return (error_t){.code = ERROR_CODE_STRIP_INIT_FAIL,
                     .message = ws2811_get_return_t_str(ws_ret)};
  }
  printf("ledd initialized\n");

  // Init socket
  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd < 0) {
    return (error_t){.code = ERROR_CODE_SOCKET_FAIL,
                     .message = "error creating socket"};
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, '\0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT_NO);

  int opt_val = 1;
  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

  if (bind(serverfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
    return (error_t){.code = ERROR_CODE_BIND_FAIL,
                     .message = "error binding to socket"};
  }

  if (listen(serverfd, 1) < 0) {
    return (error_t){.code = ERROR_CODE_LISTEN_FAIL,
                     .message = "error listening on socket"};
  }

  printf("ledd listening on port %d\n", PORT_NO);

  while (true) {
    struct sockaddr_in cli_addr;
    socklen_t client_len = sizeof(cli_addr);
    int clientfd = accept(serverfd, (struct sockaddr *) &cli_addr, &client_len);
    printf("client connected %s\n", inet_ntoa(cli_addr.sin_addr));

    if (clientfd < 0) {
      fprintf(stderr, "Could not establish new connection\n");
      break;
    }

    opc_pixel8_t pixelbuf[MAX_PIXELS];

    while (true) {
      opc_header_t opc_header;

      int read = recv(clientfd, &opc_header, sizeof(opc_header_t), 0);
      if (read == 0) { // We're done here
        close(clientfd);
        break;
      }

      uint16_t payload_length = (opc_header.length_high << 8) | opc_header.length_low;
      if (payload_length % 3 != 0) {
        fprintf(stderr, "bad payload length %d\n", payload_length);
        close(clientfd);
        break;
      }

      read = recv(clientfd, pixelbuf, payload_length, 0);
      if (read == 0) { // We're done here
        close(clientfd);
        break;
      }

      ledd_execute(pixelbuf, payload_length / 3);
    }
  }

  close(serverfd);

  return (error_t){.code = ERROR_CODE_SUCCESS, .message = NULL};
}

void ledd_shutdown(void) {
  printf("ledd shutting down\n");
  close(serverfd);
  ws2811_fini(&strip);
  printf("ledd shut down\n");
}

static void ledd_execute(opc_pixel8_t* pixels, int num_pixels) {
  uint8_t max_range = MIN(num_pixels, WRAPAROUND);
  for (int i = 0; i < max_range; i++) {
    const opc_pixel8_t px = pixels[i];
    const ws2811_led_t color = ledd_color_no_white(px.red, px.green, px.blue);
    ledd_set(i, color);
  }

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

static inline ws2811_led_t ledd_color_no_white(uint8_t red, uint8_t green, uint8_t blue) {
  // Find minimum color (white amount)
  uint8_t white = MIN(MIN(red, green), blue);
  red -= white;
  green -= white;
  blue -= white;

  return ledd_color(red, green, blue, white);
}

static inline ws2811_led_t ledd_color(uint8_t red, uint8_t green, uint8_t blue,
                                      uint8_t white) {
  // 0xWWRRGGBB
  return (((ws2811_led_t)red) << 16) | (((ws2811_led_t)green) << 8) |
         ((ws2811_led_t)blue) | (((ws2811_led_t)white) << 24);
}

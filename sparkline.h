/**
 * Copyright (c) 2024 tapiocode
 * https://github.com/tapiocode
 * MIT License
 */

#include "pico/stdlib.h"
#include "pico-ssd1306/ssd1306.h"

#define SPARKLINE_MAX_POINTS 128

typedef struct {
  uint8_t  x;               // Top-left corner x
  uint8_t  y;               // Top-left corner y
  uint8_t  area_width;      // Width of area where sparkline is drawn
  uint8_t  area_height;     // Height of area where sparkline is drawn
  uint8_t  stepsize;        // Horizontal space between datapoints

  /**
   * Values for the sparkline will be normalized automatically to fit the area.
   * Using int16_t[] to allow larger range of values (eg. temps).
   */
  int16_t  datapoints[SPARKLINE_MAX_POINTS];
  int8_t   current_datapoint; // Index of current datapoint in datapoints[]
  int16_t  min; // Smallest value in set
  int16_t  max; // Largest value in set

} sparkline_t;

void sparkline_clear(ssd1306_t *display, sparkline_t *sparkline);
void sparkline_draw(ssd1306_t *display, sparkline_t *sparkline);
uint8_t sparkline_add_datapoint(sparkline_t *sparkline, int16_t datapoint);

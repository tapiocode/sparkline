/**
 * Copyright (c) 2024 tapiocode
 * https://github.com/tapiocode
 * MIT License
 */

#include "pico/stdlib.h"
#include "pico-ssd1306/ssd1306.h"

#define SPARKLINE_MAX_POINTS 128

typedef struct {
  int32_t  x;               // Top-left corner x
  int32_t  y;               // Top-left corner y
  int32_t  area_width;      // Width of area where sparkline is drawn
  int32_t  area_height;     // Height of area where sparkline is drawn
  int32_t  stepsize;        // Horizontal space between datapoints

  /**
   * Values for the sparkline will be normalized automatically to fit the area.
   * Using int16_t[] to allow larger range of values (eg. temps).
   */
  int32_t  datapoints[SPARKLINE_MAX_POINTS];
  int32_t  current_datapoint; // Index of current datapoint in datapoints[]
  int32_t  min; // Smallest value in set
  int32_t  max; // Largest value in set

} sparkline_t;

void sparkline_clear(ssd1306_t *display, sparkline_t *sparkline);
void sparkline_draw(ssd1306_t *display, sparkline_t *sparkline);
int32_t sparkline_add_datapoint(sparkline_t *sparkline, int32_t datapoint);

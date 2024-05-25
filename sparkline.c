/**
 * Copyright (c) 2024 tapiocode
 * https://github.com/tapiocode
 * MIT License
 */

#include <math.h>
#include <stdlib.h>
#include "sparkline.h"
#include "pico-ssd1306/ssd1306.h"

/**
 * Clears the area defined by sparkline and resets datapoints
 */
void sparkline_clear(ssd1306_t *display, sparkline_t *sparkline) {
  ssd1306_clear_square(display, sparkline->x, sparkline->y, sparkline->area_width, sparkline->area_height);
  // Write the lowest possible number to datapoints to indicate empty value
  for (uint8_t i = 0; i < SPARKLINE_MAX_POINTS; i++) {
    sparkline->datapoints[i] = INT16_MIN;
  }
  sparkline->current_datapoint = -1;
}

/**
 * Adds datapoint to array, returns index of new datapoint or -1 if the array is full
 */
uint8_t sparkline_add_datapoint(sparkline_t *sparkline, int16_t datapoint) {
  if (sparkline->current_datapoint == SPARKLINE_MAX_POINTS ||
      sparkline->current_datapoint == sparkline->area_width) {
    return -1;
  }
  // Initialize or update min/max values
  if (sparkline->current_datapoint < 0) {
    sparkline->current_datapoint = 0;
    sparkline->min = INT16_MAX;
    sparkline->max = INT16_MIN;
  }
  sparkline->min = sparkline->min > datapoint ? datapoint : sparkline->min;
  sparkline->max = sparkline->max < datapoint ? datapoint : sparkline->max;
  sparkline->datapoints[sparkline->current_datapoint] = datapoint;
  sparkline->current_datapoint += 1;
  return sparkline->current_datapoint;
}

inline static void swap(uint8_t *a, uint8_t *b) {
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
}

void sparkline_draw_line(ssd1306_t *display, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  // Always draw from x1 to x2
  if (x1 > x2) {
    swap(&x1, &x2);
    swap(&y1, &y2);
  }
  int16_t delta_x = x2 - x1;
  int16_t delta_y = y2 - y1;
  uint8_t i;
  bool is_steep = delta_x < abs(delta_y);
  float slope = is_steep ? (float) delta_x / (float) delta_y : (float) delta_y / (float) delta_x;
  float k;

  // If line is steep, trace pixels along Y axis, otherwise X axis
  if (is_steep) {
    // Always draw from y1 to y2
    if (y1 > y2) {
      swap(&x1, &x2);
      swap(&y1, &y2);
    }
    for (i = y1; i < y2; i++) {
      k = slope * (i - y1) + x1;
      ssd1306_draw_pixel(display, k, i);
    }
  } else {
    for (i = x1; i <= x2; i++) {
      k = slope * (i - x1) + y1;
      ssd1306_draw_pixel(display, i, k);
    }
  }
}

/**
 * Draw configured and populated sparkline on display
 */
void sparkline_draw(ssd1306_t *display, sparkline_t *sparkline) {
  uint8_t prev_y;
  uint8_t prev_x = 0;
  uint8_t x = sparkline->area_width;
  uint8_t y;
  int16_t range = sparkline->max - sparkline->min;
  int16_t height;
  for (uint8_t i = 0; i < sparkline->current_datapoint; i++) {
    height = sparkline->datapoints[i] - sparkline->min;
    // Get normalized height within range [0, area_height]
    y = sparkline->area_height - (height * sparkline->area_height) / range;
    // Special case: if min and max are the same, center the line
    if (range == 0) {
      y = sparkline->area_height / 2;
    }
    if (i > 0) {
      x -= sparkline->stepsize;
      // Prevent drawing past left edge
      x = x < 0 ? 0 : x;
      sparkline_draw_line(
        display,
        x + sparkline->x,
        y + sparkline->y,
        prev_x + sparkline->x,
        prev_y + sparkline->y
      );
      if (x == 0) {
        break;
      }
    }
    prev_y = y;
    prev_x = x;
  }
}

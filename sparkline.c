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
int32_t sparkline_add_datapoint(sparkline_t *sparkline, int32_t datapoint) {
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

void sparkline_draw_line(ssd1306_t *display, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  /**
   * Implements Bresenham's line algorithm, see:
   * https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
   */
  int32_t D, dx, dy, step_x = 1, step_y = 1;
  dx = x2 - x1;
  dy = y2 - y1;
  if (dx < 0) {
      dx = -dx;
      step_x = -step_x;
  }

  if (dy < 0) {
      dy = -dy;
      step_y = -step_y;
  }

  ssd1306_draw_pixel(display, x1, y1);
  if (dy < dx) {
      D = dy * 2 - dx;
      while (x1 != x2) {
          x1 += step_x;
          if (D >= 0) {
              y1 += step_y;
              D -= 2 * dx;
          }
          D += 2 * dy;
          ssd1306_draw_pixel(display, x1, y1);
      }
  } else {
      D = dy - dx * 2;
      while (y1 != y2) {
          y1 += step_y;
          if (D <= 0) {
              x1 += step_x;
              D += 2*dy;
          }
          D -= 2*dx;
          ssd1306_draw_pixel(display, x1, y1);
      }
  }
}
/**
 * Draw configured and populated sparkline on display
 */
void sparkline_draw(ssd1306_t *display, sparkline_t *sparkline) {
  int32_t prev_y;
  int32_t prev_x = 0;
  int32_t x = sparkline->area_width;
  int32_t y;
  int32_t range = sparkline->max - sparkline->min;
  int32_t height;
  for (int32_t i = 0; i < sparkline->current_datapoint; i++) {
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

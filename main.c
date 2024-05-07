/**
 * Copyright (c) 2024 tapiocode
 * https://github.com/tapiocode
 * MIT License
 */

#define I2C_PORT i2c1
#define I2C_SDA 18
#define I2C_SCL 19
#define ARR_LEN(a) (sizeof(a) / sizeof(a[0]))

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico-ssd1306/ssd1306.h"
#include "sparkline.h"

static ssd1306_t display;
static sparkline_t sparkline = {
  .x = 0,
  .y = 12,
  .area_width = 100,
  .area_height = 42,
  .stepsize = 5,
};

void init_display(uint8_t pin_sda, uint8_t pin_scl) {
  i2c_init(I2C_PORT, 400*1000);
  gpio_set_function(pin_sda, GPIO_FUNC_I2C);
  gpio_set_function(pin_scl, GPIO_FUNC_I2C);
  gpio_pull_up(pin_sda);
  gpio_pull_up(pin_scl);
  ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);
}

void draw_sparkline(char* label, int16_t values[], size_t len) {
  ssd1306_clear(&display);

  // Append texts
  ssd1306_draw_string(&display, 0, 0, 1, "sparkline demo");
  uint8_t label_y = sparkline.y + sparkline.area_height + 1;
  ssd1306_clear_square(&display, 0, label_y, 128, 5);
  ssd1306_draw_string(&display, 0, label_y, 1, label);

  // Load up values
  sparkline_clear(&display, &sparkline);
  for (int i = 0; i < len; i++) {
    sparkline_add_datapoint(&sparkline, values[i]);
  }

  sparkline_draw(&display, &sparkline);

  ssd1306_show(&display);
  sleep_ms(1000);
}

int main() {
  int16_t dummy_downward[]   = { -49, -39, -32, -28, -20, -10, -4, 2, 3, 1, 5, 6, 8, 10, 12, 12, 13, 11, 14, 18, 19, 21, 22 };
  int16_t dummy_upward[] = { 22114, 19021, 18002, 17501, 17210, 16991, 16872, 16910, 15990, 15021, 14091, 12949, 10201, 8812, 7014, 5529, 4015 };
  int16_t dummy_random[]   = { 12, 22, 15, 1, -5, -13, -25, -15, -10, 0, 18, 23, 10, 28, 30 };
  int16_t dummy_constant[] = { 4, 4, 4, 4, 4 };

  stdio_init_all();
  init_display(I2C_SDA, I2C_SCL);

  while (1) {
    draw_sparkline("upward",   dummy_upward,   ARR_LEN(dummy_upward));
    draw_sparkline("downward", dummy_downward, ARR_LEN(dummy_downward));
    draw_sparkline("random",   dummy_random,   ARR_LEN(dummy_random));
    draw_sparkline("constant", dummy_constant, ARR_LEN(dummy_constant));
  }
}
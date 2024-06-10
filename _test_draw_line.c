
/**
 * Code snippet for testing purposes.
 *
 * Draws lines in all directions using various slopes to test the
 * drawing function.
 */

int main() {
  ssd1306_clear(&display);

  // Square with a dot in the middle, lines from center to edges
  ssd1306_draw_empty_square(&display, 0, 0, 40, 40);
  ssd1306_draw_pixel(&display, 20, 20);
  ssd1306_draw_line(&display, 20, 20 - 2, 20, 0 + 2); // Up
  ssd1306_draw_line(&display, 20, 20 + 2, 20, 40 - 2); // Down
  ssd1306_draw_line(&display, 20 - 2, 20, 0 + 2, 20); // Left
  ssd1306_draw_line(&display, 20 + 2, 20, 40 - 2, 20); // Right

  // Steep up and down lines, crossing
  // Left to right
  ssd1306_draw_empty_square(&display, 48, 0, 14, 63);
  ssd1306_draw_line(&display, 50, 2, 60, 61); // Down
  ssd1306_draw_line(&display, 50, 61, 60, 2); // Up
  // Right to left
  ssd1306_draw_empty_square(&display, 68, 0, 14, 63);
  ssd1306_draw_line(&display, 80, 2, 70, 61); // Down
  ssd1306_draw_line(&display, 80, 61, 70, 2); // Up

  // Slowly rising and falling lines, crossing
  // Left to right
  ssd1306_draw_empty_square(&display, 88, 0, 24, 20);
  ssd1306_draw_line(&display, 90, 2, 110, 18); // Down
  ssd1306_draw_line(&display, 90, 18, 110, 2); // Up
  // Right to left
  ssd1306_draw_empty_square(&display, 88, 30, 24, 20);
  ssd1306_draw_line(&display, 110, 32, 90, 48); // Down
  ssd1306_draw_line(&display, 110, 48, 90, 32); // Up

  ssd1306_show(&display);

  while (1) {
    tight_loop_contents();
  }
}

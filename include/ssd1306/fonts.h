#ifndef SSD1306_FONTS_H
#define SSD1306_FONTS_H

#include <stdint.h>

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t offset_y; // This helps us draw each character on the baseline
    const uint16_t *pixel_data;
} ssd1306_font_character_t;

typedef struct {
    uint8_t letter_space; // Space between letters (pixels)
    uint8_t line_height; // Line height (pixels)
    uint8_t word_space; // Space between words (pixels)
} ssd1306_font_config_t;

void ssd1306_font_init(ssd1306_font_config_t *config);

extern const ssd1306_font_config_t SSD1306_FONT_DEFAULT_CONFIG;

#endif
#include <ssd1306/ssd1306_fonts.h>

// Include our external fonts
extern const ssd1306_font_t font_default;

const ssd1306_font_t * const fonts[SSD1306_TOTAL_FONTS] = {
    [SSD1306_FONT_DEFAULT] = &font_default
};



void ssd1306_font_init(ssd1306_font_config_t *config) {
    // TODO
}
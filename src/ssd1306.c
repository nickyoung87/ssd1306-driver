#include <stddef.h>
#include <string.h>
#include <ssd1306/ssd1306.h>
#include <stdint.h>
#include <stdbool.h>

extern const ssd1306_font_t * fonts[SSD1306_TOTAL_FONTS];

int32_t ssd1306_init(ssd1306_t *device, const ssd1306_config_t *config) {
    device->vram         = config->buffer_ptr;
    device->buffer_size  = config->buffer_len;
    device->write        = config->write_cb;
    device->user_handle  = config->user_handle;
    device->width        = config->width;
    device->height       = config->height;
    device->orientation  = config->orientation;
    device->current_font = config->default_font;

    const uint8_t init_commands[] = {
        SSD1306_CMD_DISPLAY_OFF,
        SSD1306_CMD_SET_CLOCK_DIV,      0x80,
        SSD1306_CMD_SET_MULTIPLEX,      (device->height == 32) ? 0x1F : 0x3F,
        SSD1306_CMD_SET_DISPLAY_OFFSET, 0x00,
        SSD1306_CMD_SET_START_LINE,
        SSD1306_CMD_CHARGE_PUMP,        SSD1306_CMD_CHARGE_PUMP_ENABLE,
        SSD1306_CMD_MEMORY_MODE,        0x00,
        (device->orientation == SSD1306_ORIENT_NORMAL || device->orientation == SSD1306_ORIENT_90) ? SSD1306_CMD_SEGMAP_NORMAL : SSD1306_CMD_SEGMAP_REMAPPED,
        (device->orientation == SSD1306_ORIENT_NORMAL || device->orientation == SSD1306_ORIENT_90) ? SSD1306_CMD_COMSCAN_NORMAL : SSD1306_CMD_COMSCAN_REMAPPED,
        SSD1306_CMD_DISPLAY_ON
    };

    return device->write(device->user_handle, true, init_commands, sizeof(init_commands));
}

void ssd1306_update_display(ssd1306_t *device) {
    if (device->vram == NULL || device->write == NULL) return;

    uint8_t max_page = (device->height / 8) - 1;

    uint8_t reset_cursor_commands[] = {
        SSD1306_CMD_SET_COLUMN_ADDR,    0x00, (uint8_t)(device->width - 1),
        SSD1306_CMD_SET_PAGE_ADDR,      0x00, max_page
    };

    // Position cursor
    device->write(device->user_handle, true, reset_cursor_commands, sizeof(reset_cursor_commands));

    // Send our total bytes to screen right after cursor reset
    device->write(device->user_handle, false, device->vram, device->buffer_size);
}

void ssd1306_clear_display(ssd1306_t *device) {
    memset(device->vram, 0x00, device->buffer_size);
}

void ssd1306_draw_pixel(ssd1306_t *device, uint8_t x, uint8_t y, bool is_on) {
    // We can just use math to figure out rotation
    uint8_t hx = 0; // Hardware X
    uint8_t hy = 0; // Hardware Y

    // Normal Orientation
    if (device->orientation == SSD1306_ORIENT_NORMAL || device->orientation == SSD1306_ORIENT_180) {
        if (x >= device->width || y >= device->height || device->vram == NULL) return;

        hx = x;
        hy = y;
    } else if (device->orientation == SSD1306_ORIENT_90 || device->orientation == SSD1306_ORIENT_270) {
        if (x >= device->height || y >= device->width || device->vram == NULL) return;

        hx = (device->width - 1) - y;
        hy = x;
    }

    uint8_t page = hy / 8;
    uint16_t buffer_index = (page * device->width) + hx;
    uint8_t bit_index = hy % 8;

    if (is_on) {
        device->vram[buffer_index] |= (1 << bit_index);
    } else {
        device->vram[buffer_index] &= ~(1 << bit_index);
    }
}

void ssd1306_draw_character(ssd1306_t *device, uint8_t x, uint8_t y, char c) {
    
    const ssd1306_font_t *current_font = fonts[device->current_font];

    if (c >= current_font->start_ascii && c <= current_font->end_ascii) {
        ssd1306_font_character_t current_char = current_font->characters[c - current_font->start_ascii];

        for (int row = 0; row < current_char.height; row++) {
            for (int col = 0; col < current_char.width; col++) {
                bool is_on = ((current_char.pixel_data[row] >> (15 - col)) & 0x01);
                ssd1306_draw_pixel(device, x + col, y + row, is_on);
            }
        }
    }
}

void ssd1306_draw_text(ssd1306_t *device, uint8_t x, uint8_t y, char *str) {

    const ssd1306_font_t *current_font = fonts[device->current_font];

    while (*str != '\0') {
        if (*str >= current_font->start_ascii && *str <= current_font->end_ascii) {
            ssd1306_draw_character(device, x, y, *str);
            x += current_font->characters[*str - current_font->start_ascii].width + current_font->config.letter_space;
        } else {
            // For now we just treat it as a blank space if its not in our character list
            x += current_font->config.word_space;
        }
        
        str++;
    }
}

void ssd1306_display_on(ssd1306_t *device) {
    uint8_t screen_commands[] = {
        SSD1306_CMD_DISPLAY_ON
    };

    device->write(device->user_handle, true, screen_commands, sizeof(screen_commands));
}

void ssd1306_display_off(ssd1306_t *device) {
    uint8_t screen_commands[] = {
        SSD1306_CMD_DISPLAY_OFF
    };

    device->write(device->user_handle, true, screen_commands, sizeof(screen_commands));
}
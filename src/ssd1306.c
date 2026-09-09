#include <stddef.h>
#include <string.h>
#include <ssd1306/ssd1306.h>
#include <stdint.h>
#include <stdbool.h>

int32_t ssd1306_init(ssd1306_t *device, const ssd1306_config_t *config) {
    device->vram        = config->buffer_ptr;
    device->buffer_size = config->buffer_len;
    device->write       = config->write_cb;
    device->user_handle = config->user_handle;
    device->width       = config->width;
    device->height      = config->height;
    device->orientation = config->orientation;

    const uint8_t init_commands[] = {
        SSD1306_CMD_DISPLAY_OFF,
        SSD1306_CMD_SET_CLOCK_DIV,      0x80,
        SSD1306_CMD_SET_MULTIPLEX,      (device->height == 32) ? 0x1F : 0x3F,
        SSD1306_CMD_SET_DISPLAY_OFFSET, 0x00,
        SSD1306_CMD_SET_START_LINE,
        SSD1306_CMD_CHARGE_PUMP,        SSD1306_CMD_CHARGE_PUMP_ENABLE,
        SSD1306_CMD_MEMORY_MODE,        0x00,
        SSD1306_CMD_SEGMAP_REMAPPED,
        SSD1306_CMD_COMSCAN_REMAPPED,
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
    if (x >= device->width || y >= device->height || device->vram == NULL) return;

    uint8_t page = y / 8;
    uint16_t buffer_index = (page * device->width) + x;
    uint8_t bit_index = y % 8;

    if (is_on) {
        device->vram[buffer_index] |= (1 << bit_index);
    } else {
        device->vram[buffer_index] &= ~(1 << bit_index);
    }
}

void ssd1306_draw_character(ssd1306_t *device, uint8_t x, uint8_t y, char c) {
    const uint8_t characters[][8] = {
        { 0x38, 0x44, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x82 }, // A
        { 0xFC, 0x82, 0x82, 0xFE, 0x84, 0x82, 0x82, 0xFC }, // B
        { 0x3C, 0x42, 0x80, 0x80, 0x80, 0x80, 0x42, 0x3C }, // C
        { 0xF8, 0x84, 0x82, 0x82, 0x82, 0x82, 0x84, 0xF8 }, // D
        { 0xFE, 0x80, 0x80, 0xFC, 0x80, 0x80, 0x80, 0xFE }, // E
        { 0xFE, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0x80 }, // F
        { 0x7C, 0x82, 0x80, 0x80, 0x8E, 0x82, 0x82, 0x7C }, // G
        { 0x82, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x82 }, // H
        { 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFE }, // I
        { 0xFE, 0x08, 0x08, 0x08, 0x08, 0x08, 0x88, 0x70 }, // J
        { 0x82, 0x84, 0x88, 0x90, 0xE0, 0x98, 0x84, 0x82 }, // K
        { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFE }, // L
        { 0x82, 0xC6, 0xAA, 0x92, 0x82, 0x82, 0x82, 0x82 }, // M
        { 0x82, 0xC2, 0xA2, 0x92, 0x8A, 0x86, 0x82, 0x82 }, // N
        { 0x7C, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7C }, // O
        { 0xFC, 0x82, 0x82, 0x82, 0xFC, 0x80, 0x80, 0x80 }, // P
        { 0x7C, 0x82, 0x82, 0x82, 0x82, 0x8A, 0x84, 0x7A }, // Q
        { 0xF8, 0x84, 0x82, 0x82, 0xFC, 0x88, 0x84, 0x82 }, // R
        { 0x7C, 0x82, 0x80, 0x78, 0x04, 0x02, 0x82, 0x7C }, // S
        { 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 }, // T
        { 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38 }, // U
        { 0x82, 0x82, 0x82, 0x82, 0x82, 0x44, 0x28, 0x10 }, // V
        { 0x82, 0x82, 0x82, 0x92, 0x92, 0x92, 0x92, 0x6C }, // W
        { 0x82, 0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82 }, // X
        { 0x82, 0x82, 0x44, 0x38, 0x10, 0x10, 0x10, 0x10 }, // Y
        { 0xFE, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0xFE }  // Z
    };

    for (int row = 0; row < 8; row++) {
        uint8_t row_data = characters[c - 65][row];
        for (int col = 0; col < 8; col++) {
            bool is_on = (row_data >> (7 - col)) & 0x01;
            ssd1306_draw_pixel(device, x + col, y + row, is_on);
        }
    }
}

void ssd1306_draw_text(ssd1306_t *device, uint8_t x, uint8_t y, char *str) {
    while (*str != '\0') {
        if (*str >= 65 && *str <= 90) {
            ssd1306_draw_character(device, x, y, *str);
        }
        x += 8; // Character width
        str++;
    }
}
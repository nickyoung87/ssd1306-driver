#pragma once

#include <stdbool.h>
#include <stdint.h>

/* ========================================
 * SSD1306 Driver Font Types
 * ===================================== */
typedef enum {
    SSD1306_FONT_DEFAULT
} ssd1306_font_id_t;

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

/* ========================================
 * SSD1306 Driver Types
 * ===================================== */
typedef enum {
    SSD1306_ORIENT_NORMAL,
    SSD1306_ORIENT_90,
    SSD1306_ORIENT_180,
    SSD1306_ORIENT_270
} ssd1306_orientation_t;

typedef int32_t (*ssd1306_write_fn)(void *user_handle, bool is_command, const uint8_t *data, uint16_t length);

typedef struct {
    uint8_t *buffer_ptr;
    uint16_t buffer_len;
    ssd1306_write_fn write_cb;
    void *user_handle;
    uint8_t width;
    uint8_t height;
    ssd1306_orientation_t orientation;
    ssd1306_font_id_t default_font;
} ssd1306_config_t;

typedef struct {
    uint8_t *vram;
    uint16_t buffer_size;
    ssd1306_write_fn write;
    void *user_handle;
    uint8_t width;
    uint8_t height;
    ssd1306_orientation_t orientation;
    ssd1306_font_id_t current_font;
} ssd1306_t;
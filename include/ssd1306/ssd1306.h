#ifndef SSD1306_H
#define SSD1306_H

#include <stdbool.h>
#include <stdint.h>

#define SSD1306_CMD_DISPLAY_OFF          0xAE
#define SSD1306_CMD_DISPLAY_ON           0xAF

#define SSD1306_CMD_SET_CLOCK_DIV        0xD5
#define SSD1306_CMD_SET_MULTIPLEX        0xA8
#define SSD1306_CMD_SET_DISPLAY_OFFSET   0xD3
#define SSD1306_CMD_SET_START_LINE       0x40

#define SSD1306_CMD_CHARGE_PUMP          0x8D
#define SSD1306_CMD_CHARGE_PUMP_ENABLE   0x14

#define SSD1306_CMD_MEMORY_MODE          0x20
#define SSD1306_CMD_SET_COLUMN_ADDR      0x21
#define SSD1306_CMD_SET_PAGE_ADDR        0x22

#define SSD1306_CMD_SEGMAP_NORMAL        0xA0
#define SSD1306_CMD_SEGMAP_REMAPPED      0xA1

#define SSD1306_CMD_COMSCAN_NORMAL       0xC0
#define SSD1306_CMD_COMSCAN_REMAPPED     0xC8

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
} ssd1306_config_t;

typedef struct {
    uint8_t *vram;
    uint16_t buffer_size;
    ssd1306_write_fn write;
    void *user_handle;
    uint8_t width;
    uint8_t height;
    ssd1306_orientation_t orientation;
} ssd1306_t;

int32_t ssd1306_init(ssd1306_t *device, const ssd1306_config_t *config);
void ssd1306_update_display(ssd1306_t *device);
void ssd1306_clear_display(ssd1306_t *device);
void ssd1306_draw_pixel(ssd1306_t *device, uint8_t x, uint8_t y, bool is_on);
void ssd1306_draw_character(ssd1306_t *device, uint8_t x, uint8_t y, char c);
void ssd1306_draw_text(ssd1306_t *device, uint8_t x, uint8_t y, char *str);
void ssd1306_display_on(ssd1306_t *device);
void ssd1306_display_off(ssd1306_t *device);

#endif
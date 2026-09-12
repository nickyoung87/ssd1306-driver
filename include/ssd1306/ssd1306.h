#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <ssd1306/ssd1306_types.h>

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

int32_t ssd1306_init(ssd1306_t *device, const ssd1306_config_t *config);
void ssd1306_update_display(ssd1306_t *device);
void ssd1306_clear_display(ssd1306_t *device);
void ssd1306_draw_pixel(ssd1306_t *device, uint8_t x, uint8_t y, bool is_on);
void ssd1306_draw_character(ssd1306_t *device, uint8_t x, uint8_t y, char c);
void ssd1306_draw_text(ssd1306_t *device, uint8_t x, uint8_t y, char *str);
void ssd1306_display_on(ssd1306_t *device);
void ssd1306_display_off(ssd1306_t *device);
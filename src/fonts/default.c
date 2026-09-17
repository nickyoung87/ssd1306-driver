#include <ssd1306/ssd1306_fonts.h>

static const ssd1306_font_character_t default_font_characters[];

const ssd1306_font_t font_default = {
    .characters  = default_font_characters,
    .start_ascii = 'A',
    .end_ascii   = 'Z',
    .config      = { .letter_space = 1, .line_height = 10, .word_space = 4 }
};

static const ssd1306_font_character_t default_font_characters[] = {
    {
        // A
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){
            0x3800, 0x4400, 0x8200, 0x8200, 0xFE00, 0x8200, 0x8200, 0x8200
        }
    },
    {
        // B
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){
            0xFC00, 0x8200, 0x8200, 0xFE00, 0x8400, 0x8200, 0x8200, 0xFC00
        }
    },
    {
        // C
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x3C00, 0x4200, 0x8000, 0x8000, 0x8000, 0x8000, 0x4200, 0x3C00
        }
    },
    {
        // D
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xF800, 0x8400, 0x8200, 0x8200, 0x8200, 0x8200, 0x8400, 0xF800 
        }
    },
    {
        // E
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFE00, 0x8000, 0x8000, 0xFC00, 0x8000, 0x8000, 0x8000, 0xFE00 
        }
    },
    {
        // F
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFE00, 0x8000, 0x8000, 0xF800, 0x8000, 0x8000, 0x8000, 0x8000 
        }
    },
    {
        // G
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x7C00, 0x8200, 0x8000, 0x8000, 0x8E00, 0x8200, 0x8200, 0x7C00 
        }
    },
    {
        // H
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8200, 0x8200, 0xFE00, 0x8200, 0x8200, 0x8200, 0x8200 
        }
    },
    {
        // I
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFE00, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0xFE00 
        }
    },
    {
        // J
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFE00, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x8800, 0x7000 
        }
    },
    {
        // K
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8400, 0x8800, 0x9000, 0xE000, 0x9800, 0x8400, 0x8200 
        }
    },
    {
        // L
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0xFE00 
        }
    },
    {
        // M
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0xC600, 0xAA00, 0x9200, 0x8200, 0x8200, 0x8200, 0x8200 
        }
    },
    {
        // N
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0xC200, 0xA200, 0x9200, 0x8A00, 0x8600, 0x8200, 0x8200 
        }
    },
    {
        // O
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x7C00, 0x8200, 0x8200, 0x8200, 0x8200, 0x8200, 0x8200, 0x7C00
        }
    },
    {
        // P
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFC00, 0x8200, 0x8200, 0x8200, 0xFC00, 0x8000, 0x8000, 0x8000 
        }
    },
    {
        // Q
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x7C00, 0x8200, 0x8200, 0x8200, 0x8200, 0x8A00, 0x8400, 0x7A00 
        }
    },
    {
        // R
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xF800, 0x8400, 0x8200, 0x8200, 0xFC00, 0x8800, 0x8400, 0x8200 
        }
    },
    {
        // S
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x7C00, 0x8200, 0x8000, 0x7800, 0x0400, 0x0200, 0x8200, 0x7C00 
        }
    },
    {
        // T
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFE00, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000 
        }
    },
    {
        // U
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8200, 0x8200, 0x8200, 0x8200, 0x8200, 0x4400, 0x3800 
        }
    },
    {
        // V
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8200, 0x8200, 0x8200, 0x8200, 0x4400, 0x2800, 0x1000
        }
    },
    {
        // W
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8200, 0x8200, 0x9200, 0x9200, 0x9200, 0x9200, 0x6C00 
        }
    },
    {
        // X
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8200, 0x4400, 0x2800, 0x1000, 0x2800, 0x4400, 0x8200 
        }
    },
    {
        // Y
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0x8200, 0x8200, 0x4400, 0x3800, 0x1000, 0x1000, 0x1000, 0x1000 
        }
    },
    {
        // Z
        .width = 8, .height = 8, .offset_y = 0,
        .pixel_data = (const uint16_t[]){ 
            0xFE00, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0xFE00 
        }
    }
};

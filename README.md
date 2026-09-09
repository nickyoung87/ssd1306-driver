# SSD1306 OLED Driver

A lightweight, transport-agnostic C driver for the **SSD1306** 128×64 / 128×32 OLED display. The library sits between your application and the hardware by accepting a user-supplied write callback, so it works with any I2C (or SPI) implementation on any platform.

---

## Features

- **Transport-agnostic** — wire up any I2C/SPI HAL through a single write callback; no platform-specific code lives in the driver itself
- **Framebuffer model** — you own the VRAM buffer; the driver writes into it and flushes on demand
- **Display orientation** — supports Normal, 90°, 180°, and 270° rotation
- **128×64 and 128×32** display sizes
- **Pixel drawing** — set or clear individual pixels with orientation awareness
- **Text rendering** — draw uppercase characters (A–Z) with a built-in 8×8 bitmap font
- **ESP32 / ESP-IDF example** included

---

## Repository Structure

```
ssd1306-driver/
├── include/
│   └── ssd1306/
│       └── ssd1306.h        # Public API and types
├── src/
│   ├── ssd1306.c            # Driver implementation
│   └── ssd1306_fonts.c      # Font data
├── examples/
│   └── esp32_esp_idf/
│       ├── main.c           # Full ESP32 example
│       └── README.md        # ESP-IDF integration notes
├── CMakeLists.txt           # ESP-IDF component registration
└── LICENSE
```

---

## API Reference

### Types

#### `ssd1306_orientation_t`

```c
typedef enum {
    SSD1306_ORIENT_NORMAL,
    SSD1306_ORIENT_90,
    SSD1306_ORIENT_180,
    SSD1306_ORIENT_270
} ssd1306_orientation_t;
```

#### `ssd1306_write_fn`

The write callback your platform must provide:

```c
typedef int32_t (*ssd1306_write_fn)(
    void           *user_handle,  // opaque handle passed back to your callback
    bool            is_command,   // true = command byte(s), false = data byte(s)
    const uint8_t  *data,         // payload
    uint16_t        length        // payload length in bytes
);
```

Return `0` on success, any negative value on failure.

#### `ssd1306_config_t`

Passed to `ssd1306_init`:

| Field | Type | Description |
|---|---|---|
| `buffer_ptr` | `uint8_t *` | Caller-allocated VRAM buffer |
| `buffer_len` | `uint16_t` | Buffer size in bytes (`width * height / 8`) |
| `write_cb` | `ssd1306_write_fn` | Platform write callback |
| `user_handle` | `void *` | Opaque context forwarded to your callback |
| `width` | `uint8_t` | Display width in pixels (e.g. `128`) |
| `height` | `uint8_t` | Display height in pixels (e.g. `64` or `32`) |
| `orientation` | `ssd1306_orientation_t` | Display orientation |

---

### Functions

```c
int32_t ssd1306_init(ssd1306_t *device, const ssd1306_config_t *config);
```
Initialises the device struct and sends the hardware init sequence. Returns `0` on success.

---

```c
void ssd1306_clear_display(ssd1306_t *device);
```
Zeroes the VRAM buffer. Call `ssd1306_update_display` afterwards to push the blank frame to the screen.

---

```c
void ssd1306_update_display(ssd1306_t *device);
```
Resets the column/page cursor and flushes the entire VRAM buffer to the display in one transfer.

---

```c
void ssd1306_draw_pixel(ssd1306_t *device, uint8_t x, uint8_t y, bool is_on);
```
Sets (`is_on = true`) or clears a single pixel. Coordinates are in the logical space after orientation is applied — for 90°/270° rotation the logical width and height are swapped.

---

```c
void ssd1306_draw_character(ssd1306_t *device, uint8_t x, uint8_t y, char c);
```
Renders a single **uppercase** character (A–Z) using the built-in 8×8 bitmap font at position `(x, y)`.

---

```c
void ssd1306_draw_text(ssd1306_t *device, uint8_t x, uint8_t y, char *str);
```
Renders a null-terminated string of **uppercase** letters (A–Z) starting at `(x, y)`. Each character is 8 pixels wide. Non-uppercase characters are skipped but still advance the cursor.

---

## Quick Start — ESP32 / ESP-IDF

### 1. Add the component to your project

In your **project-level** `CMakeLists.txt`, point ESP-IDF at the driver directory:

```cmake
set(EXTRA_COMPONENT_DIRS "../ssd1306-driver")
```

In your **`main/CMakeLists.txt`**, declare the dependency:

```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES ssd1306-driver
)
```

### 2. Implement the write callback

The driver needs a function that knows how to talk to your specific I2C peripheral. Here is a complete implementation for the ESP-IDF `i2c_master` driver:

```c
int32_t esp32_oled_write(void *user_handle, bool is_command,
                         const uint8_t *data, uint16_t length)
{
    i2c_master_dev_handle_t dev_handle = (i2c_master_dev_handle_t)user_handle;

    uint8_t control_byte = is_command ? 0x00 : 0x40;

    uint8_t tx_buffer[length + 1];
    tx_buffer[0] = control_byte;
    memcpy(&tx_buffer[1], data, length);

    esp_err_t err = i2c_master_transmit(dev_handle, tx_buffer,
                                        length + 1, 500 / portTICK_PERIOD_MS);
    return (err == ESP_OK) ? 0 : -1;
}
```

### 3. Initialise and use the driver

```c
#include "ssd1306/ssd1306.h"

// Framebuffer: width * height / 8 bytes
static uint8_t vram[1024] = {0};  // 128 * 64 / 8

// ... set up your I2C bus and get oled_handle ...

ssd1306_t oled;

ssd1306_config_t config = {
    .width       = 128,
    .height      = 64,
    .orientation = SSD1306_ORIENT_NORMAL,
    .write_cb    = esp32_oled_write,
    .user_handle = oled_handle,   // i2c_master_dev_handle_t
    .buffer_ptr  = vram,
    .buffer_len  = sizeof(vram),
};

ssd1306_init(&oled, &config);

ssd1306_clear_display(&oled);
ssd1306_draw_text(&oled, 0, 0, "HELLO WORLD");
ssd1306_update_display(&oled);
```

See [`examples/esp32_esp_idf/main.c`](examples/esp32_esp_idf/main.c) for the full, runnable version (I2C bus setup included).

---

## Porting to Other Platforms

Only the write callback needs to change. The pattern is always the same:

1. Allocate a VRAM buffer sized `width * height / 8` bytes.
2. Implement `ssd1306_write_fn` using your platform's I2C send API — prepend `0x00` for commands and `0x40` for data.
3. Fill in `ssd1306_config_t` and call `ssd1306_init`.

---

## Current Limitations

- Text rendering supports **uppercase A–Z only**. Numbers and punctuation are not yet implemented.
- The `ssd1306_fonts.h` header (Font_7x10, Font_11x18, Font_16x26) is included for future expansion and is not yet active.

---

## License

See [LICENSE](LICENSE) for details.
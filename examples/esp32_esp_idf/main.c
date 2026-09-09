#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "ssd1306/ssd1306.h"

static const char *TAG = "OLED_MAIN";

#define I2C_SDA_PIN         21
#define I2C_SCL_PIN         22
#define OLED_I2C_ADDRESS    0x3C

static uint8_t vram[1024] = {0};

int32_t esp32_oled_write(void *user_handle, bool is_command, const uint8_t *data, uint16_t length) {
    i2c_master_dev_handle_t dev_handle = (i2c_master_dev_handle_t)user_handle;

    uint8_t control_byte = is_command ? 0x00 : 0x40;

    uint32_t tx_len = length + 1;
    uint8_t tx_buffer[tx_len];
    tx_buffer[0] = control_byte;

    memcpy(&tx_buffer[1], data, length);
    
    esp_err_t err = i2c_master_transmit(dev_handle, tx_buffer, tx_len, 500 / portTICK_PERIOD_MS);

    return (err == ESP_OK) ? 0 : -1;
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting up I2C Host Master Interface...");

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OLED_I2C_ADDRESS,
        .scl_speed_hz = 400000,
    };
    i2c_master_dev_handle_t oled_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &oled_handle));

    static ssd1306_t oled_device;

    ssd1306_config_t oled_config = {
        .width = 128,
        .height = 64,
        .orientation = SSD1306_ORIENT_NORMAL,
        .write_cb = esp32_oled_write,
        .user_handle = oled_handle,
        .buffer_ptr = vram,
        .buffer_len = sizeof(vram)
    };

    ssd1306_init(&oled_device, &oled_config);

    ssd1306_clear_display(&oled_device);
    ssd1306_draw_text(&oled_device, 0, 0, "HELLO WORLD");

    ssd1306_update_display(&oled_device);

    ESP_LOGI(TAG, "Rendering complete!");
}

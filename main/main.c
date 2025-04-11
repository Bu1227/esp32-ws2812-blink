#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"

#define LED_STRIP_GPIO 21
#define LED_STRIP_NUMS 1
#define LED_STRIP_RMT_RES_HZ (10*1000*1000)

led_strip_handle_t configure_led(void)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_NUMS,
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = LED_STRIP_RMT_RES_HZ,
        .flags.with_dma = false,
    };

    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    return led_strip;
}

void app_main(void)
{
    led_strip_handle_t led_strip = configure_led();
    uint8_t red = 255;
    uint8_t green = 11;
    uint8_t blue = 200;
    int brightness = 0;
    int step = 5; // 每次調整的亮度增量

    while (1)
    {
        // 計算當前亮度下的 RGB 值
        uint8_t scaled_red = (red * brightness) / 255;
        uint8_t scaled_green = (green * brightness) / 255;
        uint8_t scaled_blue = (blue * brightness) / 255;

        // 設定 LED 顏色
        led_strip_set_pixel(led_strip, 0, scaled_red, scaled_green, scaled_blue);
        led_strip_refresh(led_strip);

        // 調整亮度
        brightness += step;
        if (brightness >= 255 || brightness <= 0)
        {
            step = -step; // 反向調整亮度，實現漸亮漸暗
        }

        vTaskDelay(30 / portTICK_PERIOD_MS); // 控制呼吸燈速度
    }
}
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
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 50;

    while (1)
    {
        led_strip_set_pixel(led_strip, 0, red, green, blue);
        led_strip_refresh(led_strip);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led_strip_clear(led_strip);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
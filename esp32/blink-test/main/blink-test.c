#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BLINK_GPIO   2

void app_main(void)
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    uint8_t led_state = 0;

    while (1) {
        gpio_set_level(BLINK_GPIO, led_state);
        led_state = !led_state;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

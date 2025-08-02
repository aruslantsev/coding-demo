#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define LED_PIN GPIO_NUM_2
#define SENSOR_PIN GPIO_NUM_26


void app_main(void)
{
    printf("Setting pins\n");
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(SENSOR_PIN);
    gpio_set_direction(SENSOR_PIN, GPIO_MODE_INPUT);
    printf("Start\n");
    int state = gpio_get_level(SENSOR_PIN);

    while(1) {
        if (gpio_get_level(SENSOR_PIN) != state) {
            state = !state;
            gpio_set_level(LED_PIN, state);
            printf("Switched to %d\n", state);
        }
        vTaskDelay(1);
    }
}

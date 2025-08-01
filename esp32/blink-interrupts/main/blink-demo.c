#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_0

#define MAX_DELAY 1024
#define MIN_DELAY 16


void IRAM_ATTR change_delay(void *arg) {
    *((int *) arg) /= 2;
    if (*((int *) arg) < MIN_DELAY) {
        *((int *) arg) = MAX_DELAY;
    }
}

void app_main(void)
{
    int state = 0, delay = MAX_DELAY;

    printf("Setting up pins\n");
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ENABLE);
    printf("Setting up interrupts\n");
    gpio_install_isr_service(0);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
    gpio_isr_handler_add(BUTTON_PIN, change_delay, (void *) &delay);

    while(1) {
        gpio_set_level(LED_PIN, state = !state);
        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}

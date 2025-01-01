#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include <inttypes.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_timer.h"

#define LED0_BTN_PIN GPIO_NUM_2
#define LED1_BTN_PIN GPIO_NUM_4
#define BUTTON_PIN   GPIO_NUM_15
#define LED0_PIN     GPIO_NUM_18
#define LED1_PIN     GPIO_NUM_19


void led_tick(int mode)
{
    if (mode) {
        gpio_set_level(LED0_PIN, 1);
        gpio_set_level(LED1_PIN, 0);
    } else {
        gpio_set_level(LED0_PIN, 0);
        gpio_set_level(LED1_PIN, 1);
    }
}

void app_main(void)
{
    printf("Setting up pins\n");
    gpio_set_direction(LED0_BTN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED1_BTN_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED0_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);

    printf("Setting timer\n");
    uint64_t start = esp_timer_get_time();
    int is_pressed = 0, blink = 0;

    while(1) {
        if (gpio_get_level(BUTTON_PIN) != 0) {
            if (!is_pressed) {
                printf("Button pressed, low level on button pin\n");
                gpio_set_level(LED0_BTN_PIN, 1);
                gpio_set_level(LED1_BTN_PIN, 0);
                is_pressed = 1;
            }
        } else {
            if (is_pressed) {
                printf("Button released, high level on button pin\n");
                gpio_set_level(LED0_BTN_PIN, 0);
                gpio_set_level(LED1_BTN_PIN, 1);
                is_pressed = 0;
            }
        }

        vTaskDelay(1);
        uint64_t end = esp_timer_get_time();
        if (end - start >= 1000000) {
            printf("Blink: %d!\n", blink);
            led_tick(blink);
            blink = !blink;
            start = end;
        }

    }
}
